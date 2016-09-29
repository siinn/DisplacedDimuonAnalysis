#-- start of make_header -----------------

#====================================
#  Library DisplacedDimuonAnalysis
#
#   Generated Wed Sep 28 21:49:29 2016  by che.32
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_DisplacedDimuonAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DisplacedDimuonAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DisplacedDimuonAnalysis

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysis = $(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysis.make
cmt_local_tagfile_DisplacedDimuonAnalysis = $(bin)$(DisplacedDimuonAnalysis_tag)_DisplacedDimuonAnalysis.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DisplacedDimuonAnalysis_tag = $(tag)

#cmt_local_tagfile_DisplacedDimuonAnalysis = $(DisplacedDimuonAnalysis_tag).make
cmt_local_tagfile_DisplacedDimuonAnalysis = $(bin)$(DisplacedDimuonAnalysis_tag).make

endif

include $(cmt_local_tagfile_DisplacedDimuonAnalysis)
#-include $(cmt_local_tagfile_DisplacedDimuonAnalysis)

ifdef cmt_DisplacedDimuonAnalysis_has_target_tag

cmt_final_setup_DisplacedDimuonAnalysis = $(bin)setup_DisplacedDimuonAnalysis.make
cmt_dependencies_in_DisplacedDimuonAnalysis = $(bin)dependencies_DisplacedDimuonAnalysis.in
#cmt_final_setup_DisplacedDimuonAnalysis = $(bin)DisplacedDimuonAnalysis_DisplacedDimuonAnalysissetup.make
cmt_local_DisplacedDimuonAnalysis_makefile = $(bin)DisplacedDimuonAnalysis.make

else

cmt_final_setup_DisplacedDimuonAnalysis = $(bin)setup.make
cmt_dependencies_in_DisplacedDimuonAnalysis = $(bin)dependencies.in
#cmt_final_setup_DisplacedDimuonAnalysis = $(bin)DisplacedDimuonAnalysissetup.make
cmt_local_DisplacedDimuonAnalysis_makefile = $(bin)DisplacedDimuonAnalysis.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DisplacedDimuonAnalysissetup.make

#DisplacedDimuonAnalysis :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DisplacedDimuonAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DisplacedDimuonAnalysis/
#DisplacedDimuonAnalysis::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

DisplacedDimuonAnalysislibname   = $(bin)$(library_prefix)DisplacedDimuonAnalysis$(library_suffix)
DisplacedDimuonAnalysislib       = $(DisplacedDimuonAnalysislibname).a
DisplacedDimuonAnalysisstamp     = $(bin)DisplacedDimuonAnalysis.stamp
DisplacedDimuonAnalysisshstamp   = $(bin)DisplacedDimuonAnalysis.shstamp

DisplacedDimuonAnalysis :: dirs  DisplacedDimuonAnalysisLIB
	$(echo) "DisplacedDimuonAnalysis ok"

#-- end of libary_header ----------------
#-- start of library_no_static ------

#DisplacedDimuonAnalysisLIB :: $(DisplacedDimuonAnalysislib) $(DisplacedDimuonAnalysisshstamp)
DisplacedDimuonAnalysisLIB :: $(DisplacedDimuonAnalysisshstamp)
	$(echo) "DisplacedDimuonAnalysis : library ok"

$(DisplacedDimuonAnalysislib) :: $(bin)DVUtils.o $(bin)MuonEfficiency.o $(bin)DisplacedDimuonAnalysisAlg.o $(bin)DisplacedDimuonAnalysis_load.o $(bin)DisplacedDimuonAnalysis_entries.o
	$(lib_echo) "static library $@"
	$(lib_silent) cd $(bin); \
	  $(ar) $(DisplacedDimuonAnalysislib) $?
	$(lib_silent) $(ranlib) $(DisplacedDimuonAnalysislib)
	$(lib_silent) cat /dev/null >$(DisplacedDimuonAnalysisstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(DisplacedDimuonAnalysislibname).$(shlibsuffix) :: $(bin)DVUtils.o $(bin)MuonEfficiency.o $(bin)DisplacedDimuonAnalysisAlg.o $(bin)DisplacedDimuonAnalysis_load.o $(bin)DisplacedDimuonAnalysis_entries.o $(use_requirements) $(DisplacedDimuonAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) $(shlibbuilder) $(shlibflags) -o $@ $(bin)DVUtils.o $(bin)MuonEfficiency.o $(bin)DisplacedDimuonAnalysisAlg.o $(bin)DisplacedDimuonAnalysis_load.o $(bin)DisplacedDimuonAnalysis_entries.o $(DisplacedDimuonAnalysis_shlibflags)
	$(lib_silent) cat /dev/null >$(DisplacedDimuonAnalysisstamp) && \
	  cat /dev/null >$(DisplacedDimuonAnalysisshstamp)

$(DisplacedDimuonAnalysisshstamp) :: $(DisplacedDimuonAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(DisplacedDimuonAnalysislibname).$(shlibsuffix) ; then \
	  cat /dev/null >$(DisplacedDimuonAnalysisstamp) && \
	  cat /dev/null >$(DisplacedDimuonAnalysisshstamp) ; fi

DisplacedDimuonAnalysisclean ::
	$(cleanup_echo) objects DisplacedDimuonAnalysis
	$(cleanup_silent) /bin/rm -f $(bin)DVUtils.o $(bin)MuonEfficiency.o $(bin)DisplacedDimuonAnalysisAlg.o $(bin)DisplacedDimuonAnalysis_load.o $(bin)DisplacedDimuonAnalysis_entries.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)DVUtils.o $(bin)MuonEfficiency.o $(bin)DisplacedDimuonAnalysisAlg.o $(bin)DisplacedDimuonAnalysis_load.o $(bin)DisplacedDimuonAnalysis_entries.o) $(patsubst %.o,%.dep,$(bin)DVUtils.o $(bin)MuonEfficiency.o $(bin)DisplacedDimuonAnalysisAlg.o $(bin)DisplacedDimuonAnalysis_load.o $(bin)DisplacedDimuonAnalysis_entries.o) $(patsubst %.o,%.d.stamp,$(bin)DVUtils.o $(bin)MuonEfficiency.o $(bin)DisplacedDimuonAnalysisAlg.o $(bin)DisplacedDimuonAnalysis_load.o $(bin)DisplacedDimuonAnalysis_entries.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf DisplacedDimuonAnalysis_deps DisplacedDimuonAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
DisplacedDimuonAnalysisinstallname = $(library_prefix)DisplacedDimuonAnalysis$(library_suffix).$(shlibsuffix)

DisplacedDimuonAnalysis :: DisplacedDimuonAnalysisinstall ;

install :: DisplacedDimuonAnalysisinstall ;

DisplacedDimuonAnalysisinstall :: $(install_dir)/$(DisplacedDimuonAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(DisplacedDimuonAnalysisinstallname) :: $(bin)$(DisplacedDimuonAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(DisplacedDimuonAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##DisplacedDimuonAnalysisclean :: DisplacedDimuonAnalysisuninstall

uninstall :: DisplacedDimuonAnalysisuninstall ;

DisplacedDimuonAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(DisplacedDimuonAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of library_no_static ------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),DisplacedDimuonAnalysisclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DVUtils.d

$(bin)$(binobj)DVUtils.d :

$(bin)$(binobj)DVUtils.o : $(cmt_final_setup_DisplacedDimuonAnalysis)

$(bin)$(binobj)DVUtils.o : $(src)DVUtils.cxx
	$(cpp_echo) $(src)DVUtils.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(DVUtils_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(DVUtils_cppflags) $(DVUtils_cxx_cppflags)  $(src)DVUtils.cxx
endif
endif

else
$(bin)DisplacedDimuonAnalysis_dependencies.make : $(DVUtils_cxx_dependencies)

$(bin)DisplacedDimuonAnalysis_dependencies.make : $(src)DVUtils.cxx

$(bin)$(binobj)DVUtils.o : $(DVUtils_cxx_dependencies)
	$(cpp_echo) $(src)DVUtils.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(DVUtils_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(DVUtils_cppflags) $(DVUtils_cxx_cppflags)  $(src)DVUtils.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),DisplacedDimuonAnalysisclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MuonEfficiency.d

$(bin)$(binobj)MuonEfficiency.d :

$(bin)$(binobj)MuonEfficiency.o : $(cmt_final_setup_DisplacedDimuonAnalysis)

$(bin)$(binobj)MuonEfficiency.o : $(src)MuonEfficiency.cxx
	$(cpp_echo) $(src)MuonEfficiency.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(MuonEfficiency_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(MuonEfficiency_cppflags) $(MuonEfficiency_cxx_cppflags)  $(src)MuonEfficiency.cxx
endif
endif

else
$(bin)DisplacedDimuonAnalysis_dependencies.make : $(MuonEfficiency_cxx_dependencies)

$(bin)DisplacedDimuonAnalysis_dependencies.make : $(src)MuonEfficiency.cxx

$(bin)$(binobj)MuonEfficiency.o : $(MuonEfficiency_cxx_dependencies)
	$(cpp_echo) $(src)MuonEfficiency.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(MuonEfficiency_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(MuonEfficiency_cppflags) $(MuonEfficiency_cxx_cppflags)  $(src)MuonEfficiency.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),DisplacedDimuonAnalysisclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DisplacedDimuonAnalysisAlg.d

$(bin)$(binobj)DisplacedDimuonAnalysisAlg.d :

$(bin)$(binobj)DisplacedDimuonAnalysisAlg.o : $(cmt_final_setup_DisplacedDimuonAnalysis)

$(bin)$(binobj)DisplacedDimuonAnalysisAlg.o : $(src)DisplacedDimuonAnalysisAlg.cxx
	$(cpp_echo) $(src)DisplacedDimuonAnalysisAlg.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(DisplacedDimuonAnalysisAlg_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(DisplacedDimuonAnalysisAlg_cppflags) $(DisplacedDimuonAnalysisAlg_cxx_cppflags)  $(src)DisplacedDimuonAnalysisAlg.cxx
endif
endif

else
$(bin)DisplacedDimuonAnalysis_dependencies.make : $(DisplacedDimuonAnalysisAlg_cxx_dependencies)

$(bin)DisplacedDimuonAnalysis_dependencies.make : $(src)DisplacedDimuonAnalysisAlg.cxx

$(bin)$(binobj)DisplacedDimuonAnalysisAlg.o : $(DisplacedDimuonAnalysisAlg_cxx_dependencies)
	$(cpp_echo) $(src)DisplacedDimuonAnalysisAlg.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(DisplacedDimuonAnalysisAlg_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(DisplacedDimuonAnalysisAlg_cppflags) $(DisplacedDimuonAnalysisAlg_cxx_cppflags)  $(src)DisplacedDimuonAnalysisAlg.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),DisplacedDimuonAnalysisclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DisplacedDimuonAnalysis_load.d

$(bin)$(binobj)DisplacedDimuonAnalysis_load.d :

$(bin)$(binobj)DisplacedDimuonAnalysis_load.o : $(cmt_final_setup_DisplacedDimuonAnalysis)

$(bin)$(binobj)DisplacedDimuonAnalysis_load.o : $(src)components/DisplacedDimuonAnalysis_load.cxx
	$(cpp_echo) $(src)components/DisplacedDimuonAnalysis_load.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(DisplacedDimuonAnalysis_load_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(DisplacedDimuonAnalysis_load_cppflags) $(DisplacedDimuonAnalysis_load_cxx_cppflags) -I../src/components $(src)components/DisplacedDimuonAnalysis_load.cxx
endif
endif

else
$(bin)DisplacedDimuonAnalysis_dependencies.make : $(DisplacedDimuonAnalysis_load_cxx_dependencies)

$(bin)DisplacedDimuonAnalysis_dependencies.make : $(src)components/DisplacedDimuonAnalysis_load.cxx

$(bin)$(binobj)DisplacedDimuonAnalysis_load.o : $(DisplacedDimuonAnalysis_load_cxx_dependencies)
	$(cpp_echo) $(src)components/DisplacedDimuonAnalysis_load.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(DisplacedDimuonAnalysis_load_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(DisplacedDimuonAnalysis_load_cppflags) $(DisplacedDimuonAnalysis_load_cxx_cppflags) -I../src/components $(src)components/DisplacedDimuonAnalysis_load.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),DisplacedDimuonAnalysisclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DisplacedDimuonAnalysis_entries.d

$(bin)$(binobj)DisplacedDimuonAnalysis_entries.d :

$(bin)$(binobj)DisplacedDimuonAnalysis_entries.o : $(cmt_final_setup_DisplacedDimuonAnalysis)

$(bin)$(binobj)DisplacedDimuonAnalysis_entries.o : $(src)components/DisplacedDimuonAnalysis_entries.cxx
	$(cpp_echo) $(src)components/DisplacedDimuonAnalysis_entries.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(DisplacedDimuonAnalysis_entries_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(DisplacedDimuonAnalysis_entries_cppflags) $(DisplacedDimuonAnalysis_entries_cxx_cppflags) -I../src/components $(src)components/DisplacedDimuonAnalysis_entries.cxx
endif
endif

else
$(bin)DisplacedDimuonAnalysis_dependencies.make : $(DisplacedDimuonAnalysis_entries_cxx_dependencies)

$(bin)DisplacedDimuonAnalysis_dependencies.make : $(src)components/DisplacedDimuonAnalysis_entries.cxx

$(bin)$(binobj)DisplacedDimuonAnalysis_entries.o : $(DisplacedDimuonAnalysis_entries_cxx_dependencies)
	$(cpp_echo) $(src)components/DisplacedDimuonAnalysis_entries.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DisplacedDimuonAnalysis_pp_cppflags) $(lib_DisplacedDimuonAnalysis_pp_cppflags) $(DisplacedDimuonAnalysis_entries_pp_cppflags) $(use_cppflags) $(DisplacedDimuonAnalysis_cppflags) $(lib_DisplacedDimuonAnalysis_cppflags) $(DisplacedDimuonAnalysis_entries_cppflags) $(DisplacedDimuonAnalysis_entries_cxx_cppflags) -I../src/components $(src)components/DisplacedDimuonAnalysis_entries.cxx

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: DisplacedDimuonAnalysisclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DisplacedDimuonAnalysis.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DisplacedDimuonAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library DisplacedDimuonAnalysis
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)DisplacedDimuonAnalysis$(library_suffix).a $(library_prefix)DisplacedDimuonAnalysis$(library_suffix).$(shlibsuffix) DisplacedDimuonAnalysis.stamp DisplacedDimuonAnalysis.shstamp
#-- end of cleanup_library ---------------
