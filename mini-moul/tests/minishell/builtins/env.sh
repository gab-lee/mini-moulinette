#!/bin/bash

# env prints the environment; only the MS_ variables are compared.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
MS_POST=ms_post_env
ms_case "env shows the variables it was given" 'env'
ms_case "env shows an exported variable" 'export MS_NEW=1' 'env'
ms_case "env shows a changed variable" 'export MS_A=changed' 'env'
ms_case "env shows a variable with an empty value" 'export MS_EMPTY=' 'env'
ms_case "env shows two exported variables" 'export MS_ONE=1 MS_TWO=2' 'env'
ms_case "env does not show an unset variable" 'unset MS_A' 'env'
ms_case "env shows a value holding an equal sign" 'export MS_X=a=b' 'env'
ms_case "env shows a value holding a space" 'export MS_Q="hello world"' 'env'
ms_case "env does not show a variable without a value" 'export MS_NOVALUE' 'env'

exit $pc_error
