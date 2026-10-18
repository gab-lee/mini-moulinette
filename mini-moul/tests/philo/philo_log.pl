#!/usr/bin/perl
# philo_log.pl --n N --ttd T --tte E --tts S --expect alive|die|count:M
#              [--neighbors] [--partial] < log
#
# Checks a philosophers log against the subject. Prints one INFO line, then
# one PROBLEM line per rule broken (at most 6):
#   - every line is "<timestamp> <philosopher> <state>" with a state the
#     subject lists, and nothing is printed after a death
#   - a philosopher takes two forks, eats, sleeps, then thinks, and eating
#     and sleeping last at least time_to_eat and time_to_sleep
#   - no more than N/2 philosophers eat at once (and, with --neighbors, two
#     philosophers sitting side by side never eat together)
#   - a death is displayed within 10 ms of time_to_die after the last meal
#   - nobody dies unless expected; with count:M everyone ate M times
# --partial: the program was killed, so an unfinished last line is ignored.
use strict;
use warnings;
use Getopt::Long;

my ($n, $ttd, $tte, $tts, $expect, $neighbors, $partial) = (0, 0, 0, 0, 'alive', 0, 0);
GetOptions('n=i' => \$n, 'ttd=i' => \$ttd, 'tte=i' => \$tte, 'tts=i' => \$tts,
	'expect=s' => \$expect, 'neighbors' => \$neighbors, 'partial' => \$partial)
	or die "bad options\n";

my $TOL = 3;
my @problems;
sub problem { push @problems, $_[0]; }

local $/;
my $text = <STDIN>;
$text = '' unless defined $text;
my @lines = split /\n/, $text, -1;
my $last = pop @lines;
push @lines, $last if defined $last && $last ne '' && !$partial;

my %states = map { $_ => 1 } ('has taken a fork', 'is eating', 'is sleeping', 'is thinking', 'died');
my @ev;
my $lineno = 0;
my $format_ok = 1;
foreach my $line (@lines) {
	$lineno++;
	if ($line !~ /^(\d+) (\d+) (.+)$/ || !$states{$3} || $2 < 1 || $2 > $n) {
		problem("line $lineno is not \"<timestamp> <philosopher 1-$n> <state>\" with a state from the subject: \"" . substr($line, 0, 50) . "\"");
		$format_ok = 0;
		last;
	}
	push @ev, [$1, $2, $3, $lineno];
}

my ($died_id, $died_ts) = (0, 0);
my %meals;
my $total_meals = 0;
if ($format_ok) {
	my (%st, %forks, %t_eat, %t_sleep, %last_eat, %intervals);
	$st{$_} = 'idle', $forks{$_} = 0, $meals{$_} = 0 for 1 .. $n;
	my $first_ts = @ev ? $ev[0][0] : 0;
	my $died_count = 0;
	foreach my $e (@ev) {
		my ($t, $id, $k, $ln) = @$e;
		if ($died_count) {
			problem("line $ln: something was printed after a philosopher died");
			last;
		}
		if ($k eq 'died') {
			$died_count++;
			($died_id, $died_ts) = ($id, $t);
			next;
		}
		if ($st{$id} eq 'ate' && $k ne 'is sleeping') {
			problem("line $ln: philosopher $id printed \"$k\" right after \"is eating\"; \"is sleeping\" comes next");
		} elsif ($st{$id} eq 'slept' && $k ne 'is thinking') {
			problem("line $ln: philosopher $id printed \"$k\" right after \"is sleeping\"; \"is thinking\" comes next");
		} elsif ($k eq 'has taken a fork') {
			$forks{$id}++;
			problem("line $ln: philosopher $id took more than two forks for one meal") if $forks{$id} > 2;
		} elsif ($k eq 'is eating') {
			if ($forks{$id} != 2) {
				problem("line $ln: philosopher $id eats after taking $forks{$id} fork(s) instead of 2");
			}
			$forks{$id} = 0;
			$st{$id} = 'ate';
			$t_eat{$id} = $t;
			$last_eat{$id} = $t;
			$meals{$id}++;
			$total_meals++;
			push @{$intervals{$id}}, [$t, $t + $tte - $TOL];
		} elsif ($k eq 'is sleeping') {
			if ($t - $t_eat{$id} < $tte - $TOL) {
				problem("line $ln: philosopher $id slept after eating for " . ($t - $t_eat{$id}) . " ms; time_to_eat is $tte");
			}
			$st{$id} = 'slept';
			$t_sleep{$id} = $t;
		} elsif ($k eq 'is thinking') {
			if ($st{$id} eq 'slept' && $t - $t_sleep{$id} < $tts - $TOL) {
				problem("line $ln: philosopher $id thought after sleeping for " . ($t - $t_sleep{$id}) . " ms; time_to_sleep is $tts");
			}
			$st{$id} = 'thought';
		}
	}

	# At most n/2 philosophers eat at once
	my @points;
	foreach my $id (keys %intervals) {
		foreach my $iv (@{$intervals{$id}}) {
			push @points, [$iv->[0], 1], [$iv->[1], -1];
		}
	}
	my ($cur, $max, $at) = (0, 0, 0);
	foreach my $p (sort { $a->[0] <=> $b->[0] || $a->[1] <=> $b->[1] } @points) {
		$cur += $p->[1];
		($max, $at) = ($cur, $p->[0]) if $cur > $max;
	}
	if ($max > int($n / 2)) {
		problem("$max philosophers were eating at the same time (at $at ms); with $n forks at most " . int($n / 2) . " can");
	}
	if ($neighbors && $n > 1) {
		OUTER: foreach my $id (1 .. $n) {
			my $next = $id % $n + 1;
			next if $next == $id;
			foreach my $a (@{$intervals{$id} || []}) {
				foreach my $b (@{$intervals{$next} || []}) {
					if ($a->[0] < $b->[1] && $b->[0] < $a->[1]) {
						problem("philosophers $id and $next sit side by side and were eating at the same time (around $b->[0] ms): they share a fork");
						last OUTER;
					}
				}
			}
		}
	}

	if ($died_count) {
		my $base = exists $last_eat{$died_id} ? $last_eat{$died_id} : $first_ts;
		my $late = $died_ts - ($base + $ttd);
		if ($late > 10) {
			problem("philosopher $died_id died at $died_ts ms but time_to_die ended at " . ($base + $ttd) . " ms: the message came $late ms late (10 ms at most)");
		} elsif ($late < -5) {
			problem("philosopher $died_id was declared dead at $died_ts ms, " . (-$late) . " ms before time_to_die ended (" . ($base + $ttd) . " ms)");
		}
		problem("nobody should die in this setup, but philosopher $died_id died at $died_ts ms") if $expect ne 'die';
	} elsif ($expect eq 'die') {
		problem("a philosopher should have died, but no death was displayed");
	}
	if ($expect =~ /^count:(\d+)$/) {
		my $want = $1;
		foreach my $id (1 .. $n) {
			if ($meals{$id} < $want) {
				problem("philosopher $id ate $meals{$id} time(s); it must eat $want times");
				last;
			}
		}
	}
}

my $info = "INFO lines=" . scalar(@ev) . " meals=$total_meals";
$info .= " died=$died_id\@$died_ts" if $died_id;
print "$info\n";
print "PROBLEM $_\n" for @problems[0 .. ($#problems > 5 ? 5 : $#problems)];
