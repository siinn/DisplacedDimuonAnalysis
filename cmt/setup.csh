# echo "setup DisplacedDimuonAnalysis DisplacedDimuonAnalysis-00-00-00 in /n/atlas05/userdata/sche/AthAnalysisBase.2.4.42.DispDilep"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/atlas.cern.ch/repo/sw/software/AthAnalysisBase/x86_64-slc6-gcc49-opt/2.4.42/CMT/v1r25p20160527
endif
source ${CMTROOT}/mgr/setup.csh
set cmtDisplacedDimuonAnalysistempfile=`${CMTROOT}/${CMTBIN}/cmt.exe -quiet build temporary_name`
if $status != 0 then
  set cmtDisplacedDimuonAnalysistempfile=/tmp/cmt.$$
endif
${CMTROOT}/${CMTBIN}/cmt.exe setup -csh -pack=DisplacedDimuonAnalysis -version=DisplacedDimuonAnalysis-00-00-00 -path=/n/atlas05/userdata/sche/AthAnalysisBase.2.4.42.DispDilep  -no_cleanup $* >${cmtDisplacedDimuonAnalysistempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/${CMTBIN}/cmt.exe setup -csh -pack=DisplacedDimuonAnalysis -version=DisplacedDimuonAnalysis-00-00-00 -path=/n/atlas05/userdata/sche/AthAnalysisBase.2.4.42.DispDilep  -no_cleanup $* >${cmtDisplacedDimuonAnalysistempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtDisplacedDimuonAnalysistempfile}
  unset cmtDisplacedDimuonAnalysistempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtDisplacedDimuonAnalysistempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtDisplacedDimuonAnalysistempfile}
unset cmtDisplacedDimuonAnalysistempfile
exit $cmtsetupstatus

