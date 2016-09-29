#-- start of make_header -----------------

#====================================
#  Document DisplacedDimuonAnalysisConfDbMerge
#
#   Generated Wed Sep 28 21:49:50 2016  by che.32
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_DisplacedDimuonAnalysisConfDbMerge_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DisplacedDimuonAnalysisConfDbMerge_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DisplacedDimuonAnalysisConfDbMerge

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisConfDbMerge = $(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisConfDbMerge.make
cmt_local_tagfile_DisplacedDimuonAnalysisConfDbMerge = $(bin)$(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisConfDbMerge.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisConfDbMerge = $(DisplacedDimuonAnalysis_tag).make
cmt_local_tagfile_DisplacedDimuonAnalysisConfDbMerge = $(bin)$(DisplacedDimuonAnalysis_tag).make

endif

include $(cmt_local_tagfile_DisplacedDimuonAnalysisConfDbMerge)
#-include $(cmt_local_tagfile_DisplacedDimuonAnalysisConfDbMerge)

ifdef cmt_DisplacedDimuonAnalysisConfDbMerge_has_target_tag

cmt_final_setup_DisplacedDimuonAnalysisConfDbMerge = $(bin)setup_DisplacedDimuonAnalysisConfDbMerge.make
cmt_dependencies_in_DisplacedDimuonAnalysisConfDbMerge = $(bin)dependencies_DisplacedDimuonAnalysisConfDbMerge.in
#cmt_final_setup_DisplacedDimuonAnalysisConfDbMerge = $(bin)DisplacedDimuonAnalysis_DisplacedDimuonAnalysisConfDbMergesetup.make
cmt_local_DisplacedDimuonAnalysisConfDbMerge_makefile = $(bin)DisplacedDimuonAnalysisConfDbMerge.make

else

cmt_final_setup_DisplacedDimuonAnalysisConfDbMerge = $(bin)setup.make
cmt_dependencies_in_DisplacedDimuonAnalysisConfDbMerge = $(bin)dependencies.in
#cmt_final_setup_DisplacedDimuonAnalysisConfDbMerge = $(bin)DisplacedDimuonAnalysissetup.make
cmt_local_DisplacedDimuonAnalysisConfDbMerge_makefile = $(bin)DisplacedDimuonAnalysisConfDbMerge.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DisplacedDimuonAnalysissetup.make

#DisplacedDimuonAnalysisConfDbMerge :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DisplacedDimuonAnalysisConfDbMerge'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DisplacedDimuonAnalysisConfDbMerge/
#DisplacedDimuonAnalysisConfDbMerge::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
# File: cmt/fragments/merge_genconfDb_header
# Author: Sebastien Binet (binet@cern.ch)

# Makefile fragment to merge a <library>.confdb file into a single
# <project>.confdb file in the (lib) install area

.PHONY: DisplacedDimuonAnalysisConfDbMerge DisplacedDimuonAnalysisConfDbMergeclean

# default is already '#'
#genconfDb_comment_char := "'#'"

instdir      := ${CMTINSTALLAREA}/$(tag)
confDbRef    := /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis/DisplacedDimuonAnalysis.confdb
stampConfDb  := $(confDbRef).stamp
mergedConfDb := $(instdir)/lib/$(project).confdb

DisplacedDimuonAnalysisConfDbMerge :: $(stampConfDb) $(mergedConfDb)
	@:

.NOTPARALLEL : $(stampConfDb) $(mergedConfDb)

$(stampConfDb) $(mergedConfDb) :: $(confDbRef)
	@echo "Running merge_genconfDb  DisplacedDimuonAnalysisConfDbMerge"
	$(merge_genconfDb_cmd) \
          --do-merge \
          --input-file $(confDbRef) \
          --merged-file $(mergedConfDb)

DisplacedDimuonAnalysisConfDbMergeclean ::
	$(cleanup_silent) $(merge_genconfDb_cmd) \
          --un-merge \
          --input-file $(confDbRef) \
          --merged-file $(mergedConfDb)	;
	$(cleanup_silent) $(remove_command) $(stampConfDb)
libDisplacedDimuonAnalysis_so_dependencies = ../x86_64-slc6-gcc49-opt/libDisplacedDimuonAnalysis.so
#-- start of cleanup_header --------------

clean :: DisplacedDimuonAnalysisConfDbMergeclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DisplacedDimuonAnalysisConfDbMerge.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DisplacedDimuonAnalysisConfDbMergeclean ::
#-- end of cleanup_header ---------------
