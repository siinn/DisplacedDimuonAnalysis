#-- start of make_header -----------------

#====================================
#  Document DisplacedDimuonAnalysisComponentsList
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

cmt_DisplacedDimuonAnalysisComponentsList_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DisplacedDimuonAnalysisComponentsList_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DisplacedDimuonAnalysisComponentsList

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisComponentsList = $(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisComponentsList.make
cmt_local_tagfile_DisplacedDimuonAnalysisComponentsList = $(bin)$(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisComponentsList.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisComponentsList = $(DisplacedDimuonAnalysis_tag).make
cmt_local_tagfile_DisplacedDimuonAnalysisComponentsList = $(bin)$(DisplacedDimuonAnalysis_tag).make

endif

include $(cmt_local_tagfile_DisplacedDimuonAnalysisComponentsList)
#-include $(cmt_local_tagfile_DisplacedDimuonAnalysisComponentsList)

ifdef cmt_DisplacedDimuonAnalysisComponentsList_has_target_tag

cmt_final_setup_DisplacedDimuonAnalysisComponentsList = $(bin)setup_DisplacedDimuonAnalysisComponentsList.make
cmt_dependencies_in_DisplacedDimuonAnalysisComponentsList = $(bin)dependencies_DisplacedDimuonAnalysisComponentsList.in
#cmt_final_setup_DisplacedDimuonAnalysisComponentsList = $(bin)DisplacedDimuonAnalysis_DisplacedDimuonAnalysisComponentsListsetup.make
cmt_local_DisplacedDimuonAnalysisComponentsList_makefile = $(bin)DisplacedDimuonAnalysisComponentsList.make

else

cmt_final_setup_DisplacedDimuonAnalysisComponentsList = $(bin)setup.make
cmt_dependencies_in_DisplacedDimuonAnalysisComponentsList = $(bin)dependencies.in
#cmt_final_setup_DisplacedDimuonAnalysisComponentsList = $(bin)DisplacedDimuonAnalysissetup.make
cmt_local_DisplacedDimuonAnalysisComponentsList_makefile = $(bin)DisplacedDimuonAnalysisComponentsList.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DisplacedDimuonAnalysissetup.make

#DisplacedDimuonAnalysisComponentsList :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DisplacedDimuonAnalysisComponentsList'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DisplacedDimuonAnalysisComponentsList/
#DisplacedDimuonAnalysisComponentsList::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
##
componentslistfile = DisplacedDimuonAnalysis.components
COMPONENTSLIST_DIR = ../$(tag)
fulllibname = libDisplacedDimuonAnalysis.$(shlibsuffix)

DisplacedDimuonAnalysisComponentsList :: ${COMPONENTSLIST_DIR}/$(componentslistfile)
	@:

${COMPONENTSLIST_DIR}/$(componentslistfile) :: $(bin)$(fulllibname)
	@echo 'Generating componentslist file for $(fulllibname)'
	cd ../$(tag);$(listcomponents_cmd) --output ${COMPONENTSLIST_DIR}/$(componentslistfile) $(fulllibname)

install :: DisplacedDimuonAnalysisComponentsListinstall
DisplacedDimuonAnalysisComponentsListinstall :: DisplacedDimuonAnalysisComponentsList

uninstall :: DisplacedDimuonAnalysisComponentsListuninstall
DisplacedDimuonAnalysisComponentsListuninstall :: DisplacedDimuonAnalysisComponentsListclean

DisplacedDimuonAnalysisComponentsListclean ::
	@echo 'Deleting $(componentslistfile)'
	@rm -f ${COMPONENTSLIST_DIR}/$(componentslistfile)

#-- start of cleanup_header --------------

clean :: DisplacedDimuonAnalysisComponentsListclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DisplacedDimuonAnalysisComponentsList.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DisplacedDimuonAnalysisComponentsListclean ::
#-- end of cleanup_header ---------------
