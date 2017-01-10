# echo "cleanup DisplacedDimuonAnalysis DisplacedDimuonAnalysis-00-00-00 in /n/atlas05/userdata/sche/2.4.21.DV_Analysis"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/atlas.cern.ch/repo/sw/software/AthAnalysisBase/x86_64-slc6-gcc49-opt/2.4.24/CMT/v1r25p20160527; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDisplacedDimuonAnalysistempfile=`${CMTROOT}/${CMTBIN}/cmt.exe -quiet build temporary_name`
if test ! $? = 0 ; then cmtDisplacedDimuonAnalysistempfile=/tmp/cmt.$$; fi
${CMTROOT}/${CMTBIN}/cmt.exe cleanup -sh -pack=DisplacedDimuonAnalysis -version=DisplacedDimuonAnalysis-00-00-00 -path=/n/atlas05/userdata/sche/2.4.21.DV_Analysis  $* >${cmtDisplacedDimuonAnalysistempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/${CMTBIN}/cmt.exe cleanup -sh -pack=DisplacedDimuonAnalysis -version=DisplacedDimuonAnalysis-00-00-00 -path=/n/atlas05/userdata/sche/2.4.21.DV_Analysis  $* >${cmtDisplacedDimuonAnalysistempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtDisplacedDimuonAnalysistempfile}
  unset cmtDisplacedDimuonAnalysistempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtDisplacedDimuonAnalysistempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtDisplacedDimuonAnalysistempfile}
unset cmtDisplacedDimuonAnalysistempfile
return $cmtcleanupstatus

