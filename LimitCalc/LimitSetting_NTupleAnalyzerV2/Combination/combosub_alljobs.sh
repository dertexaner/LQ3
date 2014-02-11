#!/bin/sh
bsub -R "pool>10000" -o /dev/null -e /dev/null -q 2nd -J jobcombo_combo < combosub_combo.csh
bsub -R "pool>10000" -o /dev/null -e /dev/null -q 2nd -J jobcombo_mumu < combosub_mumu.csh
bsub -R "pool>10000" -o /dev/null -e /dev/null -q 2nd -J jobcombo_munu < combosub_munu.csh

#bsub -q 2nd -J jobcombo_combo < combosub_combo.csh
#bsub -q 2nd -J jobcombo_mumu < combosub_mumu.csh
#bsub -q 2nd -J jobcombo_munu < combosub_munu.csh
