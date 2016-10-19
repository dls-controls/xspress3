#!/bin/bash

# Generated run script for cs-studio.
prefix=${CONVERTER_ROOT:-/dls_sw/work/common/CSS/shadow/}

if [[ -z $1 ]]; then
    echo "No OPI specified to launch!"
    exit 1
fi

# pop the opifile off the head of the argument list
opifile="$1"
shift

CSS_RUN_SCRIPT=$(configure-ioc s -p CSS-release-gui)

project=Xspress3_2-1dls2
module=Xspress3
launch_opi=/$project/$module/$opifile

links="${prefix}/dls_sw/prod/R3.14.12.3/support/ADBinaries/2-2dls3/ADBinariesApp/opi/opi=/Xspress3_2-1dls2/ADBinaries,\
${prefix}/dls_sw/prod/R3.14.12.3/support/sscan/2-8-2/sscanApp/opi/opi=/Xspress3_2-1dls2/sscan,\
${prefix}/dls_sw/prod/R3.14.12.3/support/ADCore/2-4dls7/ADCoreApp/opi/opi=/Xspress3_2-1dls2/ADCore,\
${prefix}/dls_sw/prod/R3.14.12.3/support/Xspress3/2-1dls2/xspress3App/opi/opi=/Xspress3_2-1dls2/Xspress3"

$CSS_RUN_SCRIPT -o ${launch_opi} -s -l "$links" "$@"
