#-- start of make_header -----------------

#====================================
#  Document DisplacedDimuonAnalysisCLIDDB
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

cmt_DisplacedDimuonAnalysisCLIDDB_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DisplacedDimuonAnalysisCLIDDB_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DisplacedDimuonAnalysisCLIDDB

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisCLIDDB = $(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisCLIDDB.make
cmt_local_tagfile_DisplacedDimuonAnalysisCLIDDB = $(bin)$(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysisCLIDDB.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysisCLIDDB = $(DisplacedDimuonAnalysis_tag).make
cmt_local_tagfile_DisplacedDimuonAnalysisCLIDDB = $(bin)$(DisplacedDimuonAnalysis_tag).make

endif

include $(cmt_local_tagfile_DisplacedDimuonAnalysisCLIDDB)
#-include $(cmt_local_tagfile_DisplacedDimuonAnalysisCLIDDB)

ifdef cmt_DisplacedDimuonAnalysisCLIDDB_has_target_tag

cmt_final_setup_DisplacedDimuonAnalysisCLIDDB = $(bin)setup_DisplacedDimuonAnalysisCLIDDB.make
cmt_dependencies_in_DisplacedDimuonAnalysisCLIDDB = $(bin)dependencies_DisplacedDimuonAnalysisCLIDDB.in
#cmt_final_setup_DisplacedDimuonAnalysisCLIDDB = $(bin)DisplacedDimuonAnalysis_DisplacedDimuonAnalysisCLIDDBsetup.make
cmt_local_DisplacedDimuonAnalysisCLIDDB_makefile = $(bin)DisplacedDimuonAnalysisCLIDDB.make

else

cmt_final_setup_DisplacedDimuonAnalysisCLIDDB = $(bin)setup.make
cmt_dependencies_in_DisplacedDimuonAnalysisCLIDDB = $(bin)dependencies.in
#cmt_final_setup_DisplacedDimuonAnalysisCLIDDB = $(bin)DisplacedDimuonAnalysissetup.make
cmt_local_DisplacedDimuonAnalysisCLIDDB_makefile = $(bin)DisplacedDimuonAnalysisCLIDDB.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DisplacedDimuonAnalysissetup.make

#DisplacedDimuonAnalysisCLIDDB :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DisplacedDimuonAnalysisCLIDDB'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DisplacedDimuonAnalysisCLIDDB/
#DisplacedDimuonAnalysisCLIDDB::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
# File: cmt/fragments/genCLIDDB_header
# Author: Paolo Calafiura
# derived from genconf_header

# Use genCLIDDB_cmd to create package clid.db files

.PHONY: DisplacedDimuonAnalysisCLIDDB DisplacedDimuonAnalysisCLIDDBclean

outname := clid.db
cliddb  := DisplacedDimuonAnalysis_$(outname)
instdir := $(CMTINSTALLAREA)/share
result  := $(instdir)/$(cliddb)
product := $(instdir)/$(outname)
conflib := $(bin)$(library_prefix)DisplacedDimuonAnalysis.$(shlibsuffix)

DisplacedDimuonAnalysisCLIDDB :: $(result)

$(instdir) :
	$(mkdir) -p $(instdir)

$(result) : $(conflib) $(product)
	@$(genCLIDDB_cmd) -p DisplacedDimuonAnalysis -i$(product) -o $(result)

$(product) : $(instdir)
	touch $(product)

DisplacedDimuonAnalysisCLIDDBclean ::
	$(cleanup_silent) $(uninstall_command) $(product) $(result)
	$(cleanup_silent) $(cmt_uninstallarea_command) $(product) $(result)

#-- start of cleanup_header --------------

clean :: DisplacedDimuonAnalysisCLIDDBclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DisplacedDimuonAnalysisCLIDDB.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DisplacedDimuonAnalysisCLIDDBclean ::
#-- end of cleanup_header ---------------
