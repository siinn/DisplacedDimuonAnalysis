#-- start of make_header -----------------

#====================================
#  Document DisplacedDimuonAnalysisConf
#
#   Generated Wed Sep 28 21:49:48 2016  by che.32
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_DisplacedDimuonAnalysisConf_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DisplacedDimuonAnalysisConf_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DisplacedDimuonAnalysisConf

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisConf = $(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisConf.make
cmt_local_tagfile_DisplacedDimuonAnalysisConf = $(bin)$(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisConf.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisConf = $(DisplacedDimuonAnalysis_tag).make
cmt_local_tagfile_DisplacedDimuonAnalysisConf = $(bin)$(DisplacedDimuonAnalysis_tag).make

endif

include $(cmt_local_tagfile_DisplacedDimuonAnalysisConf)
#-include $(cmt_local_tagfile_DisplacedDimuonAnalysisConf)

ifdef cmt_DisplacedDimuonAnalysisConf_has_target_tag

cmt_final_setup_DisplacedDimuonAnalysisConf = $(bin)setup_DisplacedDimuonAnalysisConf.make
cmt_dependencies_in_DisplacedDimuonAnalysisConf = $(bin)dependencies_DisplacedDimuonAnalysisConf.in
#cmt_final_setup_DisplacedDimuonAnalysisConf = $(bin)DisplacedDimuonAnalysis_DisplacedDimuonAnalysisConfsetup.make
cmt_local_DisplacedDimuonAnalysisConf_makefile = $(bin)DisplacedDimuonAnalysisConf.make

else

cmt_final_setup_DisplacedDimuonAnalysisConf = $(bin)setup.make
cmt_dependencies_in_DisplacedDimuonAnalysisConf = $(bin)dependencies.in
#cmt_final_setup_DisplacedDimuonAnalysisConf = $(bin)DisplacedDimuonAnalysissetup.make
cmt_local_DisplacedDimuonAnalysisConf_makefile = $(bin)DisplacedDimuonAnalysisConf.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DisplacedDimuonAnalysissetup.make

#DisplacedDimuonAnalysisConf :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DisplacedDimuonAnalysisConf'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DisplacedDimuonAnalysisConf/
#DisplacedDimuonAnalysisConf::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
# File: cmt/fragments/genconfig_header
# Author: Wim Lavrijsen (WLavrijsen@lbl.gov)

# Use genconf.exe to create configurables python modules, then have the
# normal python install procedure take over.

.PHONY: DisplacedDimuonAnalysisConf DisplacedDimuonAnalysisConfclean

confpy  := DisplacedDimuonAnalysisConf.py
conflib := $(bin)$(library_prefix)DisplacedDimuonAnalysis.$(shlibsuffix)
confdb  := DisplacedDimuonAnalysis.confdb
instdir := $(CMTINSTALLAREA)$(shared_install_subdir)/python/$(package)
product := $(instdir)/$(confpy)
initpy  := $(instdir)/__init__.py

ifdef GENCONF_ECHO
genconf_silent =
else
genconf_silent = $(silent)
endif

DisplacedDimuonAnalysisConf :: DisplacedDimuonAnalysisConfinstall

install :: DisplacedDimuonAnalysisConfinstall

DisplacedDimuonAnalysisConfinstall : /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis/$(confpy)
	@echo "Installing /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis in /n/atlas05/userdata/sche/2.4.18.DV_Analysis/InstallArea/python" ; \
	 $(install_command) --exclude="*.py?" --exclude="__init__.py" --exclude="*.confdb" /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis /n/atlas05/userdata/sche/2.4.18.DV_Analysis/InstallArea/python ; \

/n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis/$(confpy) : $(conflib) /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis
	$(genconf_silent) $(genconfig_cmd)   -o /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis -p $(package) \
	  --configurable-module=GaudiKernel.Proxy \
	  --configurable-default-name=Configurable.DefaultName \
	  --configurable-algorithm=ConfigurableAlgorithm \
	  --configurable-algtool=ConfigurableAlgTool \
	  --configurable-auditor=ConfigurableAuditor \
          --configurable-service=ConfigurableService \
	  -i ../$(tag)/$(library_prefix)DisplacedDimuonAnalysis.$(shlibsuffix)

/n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis:
	@ if [ ! -d /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis ] ; then mkdir -p /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis ; fi ;

DisplacedDimuonAnalysisConfclean :: DisplacedDimuonAnalysisConfuninstall
	$(cleanup_silent) $(remove_command) /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis/$(confpy) /n/atlas05/userdata/sche/2.4.18.DV_Analysis/DisplacedDimuonAnalysis/genConf/DisplacedDimuonAnalysis/$(confdb)

uninstall :: DisplacedDimuonAnalysisConfuninstall

DisplacedDimuonAnalysisConfuninstall ::
	@$(uninstall_command) /n/atlas05/userdata/sche/2.4.18.DV_Analysis/InstallArea/python
libDisplacedDimuonAnalysis_so_dependencies = ../x86_64-slc6-gcc49-opt/libDisplacedDimuonAnalysis.so
#-- start of cleanup_header --------------

clean :: DisplacedDimuonAnalysisConfclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DisplacedDimuonAnalysisConf.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DisplacedDimuonAnalysisConfclean ::
#-- end of cleanup_header ---------------
