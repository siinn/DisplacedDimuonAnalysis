#-- start of make_header -----------------

#====================================
#  Document DisplacedDimuonAnalysisMergeComponentsList
#
#   Generated Wed Sep 28 21:49:51 2016  by che.32
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_DisplacedDimuonAnalysisMergeComponentsList_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DisplacedDimuonAnalysisMergeComponentsList_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DisplacedDimuonAnalysisMergeComponentsList

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisMergeComponentsList = $(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisMergeComponentsList.make
cmt_local_tagfile_DisplacedDimuonAnalysisMergeComponentsList = $(bin)$(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisMergeComponentsList.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisMergeComponentsList = $(DisplacedDimuonAnalysis_tag).make
cmt_local_tagfile_DisplacedDimuonAnalysisMergeComponentsList = $(bin)$(DisplacedDimuonAnalysis_tag).make

endif

include $(cmt_local_tagfile_DisplacedDimuonAnalysisMergeComponentsList)
#-include $(cmt_local_tagfile_DisplacedDimuonAnalysisMergeComponentsList)

ifdef cmt_DisplacedDimuonAnalysisMergeComponentsList_has_target_tag

cmt_final_setup_DisplacedDimuonAnalysisMergeComponentsList = $(bin)setup_DisplacedDimuonAnalysisMergeComponentsList.make
cmt_dependencies_in_DisplacedDimuonAnalysisMergeComponentsList = $(bin)dependencies_DisplacedDimuonAnalysisMergeComponentsList.in
#cmt_final_setup_DisplacedDimuonAnalysisMergeComponentsList = $(bin)DisplacedDimuonAnalysis_DisplacedDimuonAnalysisMergeComponentsListsetup.make
cmt_local_DisplacedDimuonAnalysisMergeComponentsList_makefile = $(bin)DisplacedDimuonAnalysisMergeComponentsList.make

else

cmt_final_setup_DisplacedDimuonAnalysisMergeComponentsList = $(bin)setup.make
cmt_dependencies_in_DisplacedDimuonAnalysisMergeComponentsList = $(bin)dependencies.in
#cmt_final_setup_DisplacedDimuonAnalysisMergeComponentsList = $(bin)DisplacedDimuonAnalysissetup.make
cmt_local_DisplacedDimuonAnalysisMergeComponentsList_makefile = $(bin)DisplacedDimuonAnalysisMergeComponentsList.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DisplacedDimuonAnalysissetup.make

#DisplacedDimuonAnalysisMergeComponentsList :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DisplacedDimuonAnalysisMergeComponentsList'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DisplacedDimuonAnalysisMergeComponentsList/
#DisplacedDimuonAnalysisMergeComponentsList::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
# File: cmt/fragments/merge_componentslist_header
# Author: Sebastien Binet (binet@cern.ch)

# Makefile fragment to merge a <library>.components file into a single
# <project>.components file in the (lib) install area
# If no InstallArea is present the fragment is dummy


.PHONY: DisplacedDimuonAnalysisMergeComponentsList DisplacedDimuonAnalysisMergeComponentsListclean

# default is already '#'
#genmap_comment_char := "'#'"

componentsListRef    := ../$(tag)/DisplacedDimuonAnalysis.components

ifdef CMTINSTALLAREA
componentsListDir    := ${CMTINSTALLAREA}/$(tag)/lib
mergedComponentsList := $(componentsListDir)/$(project).components
stampComponentsList  := $(componentsListRef).stamp
else
componentsListDir    := ../$(tag)
mergedComponentsList :=
stampComponentsList  :=
endif

DisplacedDimuonAnalysisMergeComponentsList :: $(stampComponentsList) $(mergedComponentsList)
	@:

.NOTPARALLEL : $(stampComponentsList) $(mergedComponentsList)

$(stampComponentsList) $(mergedComponentsList) :: $(componentsListRef)
	@echo "Running merge_componentslist  DisplacedDimuonAnalysisMergeComponentsList"
	$(merge_componentslist_cmd) --do-merge \
         --input-file $(componentsListRef) --merged-file $(mergedComponentsList)

DisplacedDimuonAnalysisMergeComponentsListclean ::
	$(cleanup_silent) $(merge_componentslist_cmd) --un-merge \
         --input-file $(componentsListRef) --merged-file $(mergedComponentsList) ;
	$(cleanup_silent) $(remove_command) $(stampComponentsList)
libDisplacedDimuonAnalysis_so_dependencies = ../x86_64-slc6-gcc49-opt/libDisplacedDimuonAnalysis.so
#-- start of cleanup_header --------------

clean :: DisplacedDimuonAnalysisMergeComponentsListclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DisplacedDimuonAnalysisMergeComponentsList.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DisplacedDimuonAnalysisMergeComponentsListclean ::
#-- end of cleanup_header ---------------
