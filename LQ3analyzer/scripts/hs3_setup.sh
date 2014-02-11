gcc_config_version=4.3.6
mpfr_config_version=2.4.2
gmp_config_version=4.3.2
LCGPLAT=x86_64-slc5-gcc43-opt
LCG_lib_name=lib64

XROOT_PATH=/afs/cern.ch/sw/lcg/external/xrootd/3.2.6/${LCGPLAT}

if [ -z $1 ] 
then LCG_contdir=/afs/cern.ch/sw/lcg/contrib
else LCG_contdir=$1 
fi
LCG_gcc_home=${LCG_contdir}/gcc/${gcc_config_version}/${LCGPLAT}
LCG_mpfr_home=${LCG_contdir}/mpfr/${mpfr_config_version}/${LCGPLAT}
LCG_gmp_home=${LCG_contdir}/gmp/${gmp_config_version}/${LCGPLAT}

#export PATH=${LCG_gcc_home}/bin:${PATH}
export PATH=${LCG_gcc_home}/bin:${PATH}:${XROOT_PATH}/bin

if [ ${LD_LIBRARY_PATH} ] 
then
#export LD_LIBRARY_PATH=${LCG_gcc_home}/${LCG_lib_name}:${LCG_mpfr_home}/lib:${LCG_gmp_home}/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${LCG_gcc_home}/${LCG_lib_name}:${LCG_mpfr_home}/lib:${LCG_gmp_home}/lib:${LD_LIBRARY_PATH}:${XROOT_PATH}/${LCG_lib_name}
else
#export LD_LIBRARY_PATH=${LCG_gcc_home}/${LCG_lib_name}:${LCG_mpfr_home}/lib:${LCG_gmp_home}/lib
export LD_LIBRARY_PATH=${LCG_gcc_home}/${LCG_lib_name}:${LCG_mpfr_home}/lib:${LCG_gmp_home}/lib:${XROOT_PATH}/${LCG_lib_name}
fi

echo
echo "-- hs3_setup.sh --"
echo "LCG_gcc_home: "$LCG_gcc_home
echo "LCG_mpfr_home: "$LCG_mpfr_home
echo "LCG_gmp_home: "$LCG_gmp_home
echo "XROOT_PATH: "$XROOT_PATH
echo "PATH: "$PATH
echo "LD_LIBRARY_PATH: "$LD_LIBRARY_PATH
echo "-- hs3_setup.sh --"
echo
