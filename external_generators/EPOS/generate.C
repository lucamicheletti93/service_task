// Macro to generate macros for the ALICE TRAIN
// Authors: Andrea Gheata, Jan Fiete Grosse-Oetringhaus, Costin Grigoras, Markus Zimmermann
//
//
// requires env variables
#include <fstream>
#include <TSystem.h>
#include <TROOT.h>
#include <TObjArray.h>
#include <TRegexp.h>
#include <TFile.h>
#include <TGrid.h>
#include <TGridResult.h>

#include "AliAnalysisTaskMyTask.h"

//#include <AliAnalysisManager.h>
//#include <AliAnalysisTaskCfg.h>
//#include <AliAnalysisAlien.h>

Int_t CopyLocalDataset(const char *griddir, const char *pattern, Int_t nfiles, const char *output, const char *archivefile, const char *outputdir);

void generate(const char *module = "__ALL__")
{
  Int_t nFiles = 2;                                                                               //TString(gSystem->Getenv("TEST_FILES_NO")).Atoi();//TEST_FILES_NO
  Int_t nTestEvents = TString(gSystem->Getenv("TEST_FILES_NO")).Atoi();                           //for the on the fly trains this contains the number of test events
  TString dataAnchor(gSystem->Getenv("FILE_PATTERN"));                                            //FILE_PATTERN
  Int_t splitMaxInputFileNumber = TString(gSystem->Getenv("SPLIT_MAX_INPUT_FILE_NUMBER")).Atoi(); //SPLIT_MAX_INPUT_FILE_NUMBER
  Int_t maxMergeFiles = TString(gSystem->Getenv("MAX_MERGE_FILES")).Atoi();                       //MAX_MERGE_FILES
  Int_t debugLevel = TString(gSystem->Getenv("DEBUG_LEVEL")).Atoi();                              //DEBUG_LEVEL
  Int_t ttl = TString(gSystem->Getenv("TTL")).Atoi();                                             //TTL
  TString excludeFiles(gSystem->Getenv("EXCLUDE_FILES"));                                         //EXCLUDE_FILES
  TString friendChainNames(gSystem->Getenv("FRIEND_CHAIN_NAMES"));                                //FRIEND_CHAIN_NAMES
  TString friendChainLibraries(gSystem->Getenv("FRIEND_CHAIN_LIBRARIES"));                        //FRIEND_CHAIN_LIBRARIES
  TString additionalpackages(gSystem->Getenv("ADDITIONAL_PACKAGES"));                             //ADDITIONAL_PACKAGES
  TString periodName(gSystem->Getenv("PERIOD_NAME"));                                             //dataset name
  Int_t trainId = TString(gSystem->Getenv("TRAIN_ID")).Atoi();                                    //train id
  Int_t trainRunId = TString(gSystem->Getenv("TRAIN_RUN_ID")).Atoi();
  Int_t iAOD = TString(gSystem->Getenv("AOD")).Atoi();         //AOD: 0 ESD, 1 AOD, 2 AOD produced together with ESD, 3 Kinematics only, 4 ESD (special WSDD production) 5 ESD cpass1 (Barrel), 6 ESD cpass1 (Outer)


  ///
  #if !defined (__CINT__) || defined (__CLING__)
      gInterpreter->LoadMacro("AliAnalysisTaskMyTask.cxx++g");
      AliAnalysisTaskMyTask *task = reinterpret_cast<AliAnalysisTaskMyTask*>(gInterpreter->ExecuteMacro("AddMyTask.C"));
  #else
      gROOT->LoadMacro("AliAnalysisTaskMyTask.cxx++g");
      gROOT->LoadMacro("AddMyTask.C");
      AliAnalysisTaskMyTask *task = AddMyTask();
  #endif
  ///



  TString dataBasePath_array[50];   //stores path to the test files
  int index = 0;
  int index_read = 0;
  dataBasePath_array[0] = gSystem->Getenv("TEST_DIR");

  do {
    index++;//index 0 contains the parent dataset
    index_read++;
    dataBasePath_array[index] = (gSystem->Getenv(Form("TEST_DIR_child_%i", index_read)));

    if(dataBasePath_array[index].EqualTo("-1") || (index>1 && dataBasePath_array[index].EqualTo(dataBasePath_array[index-1]))){
      index--;//overwrite this index in the next loop because the child is deactivated
    }
  } while (index==0 ||(dataBasePath_array[index].Data()!=NULL && dataBasePath_array[index].Length()>1));

  const char *train_name = "lego_train";

  Bool_t generateProduction = kFALSE;

  if (strcmp(module, "__ALL__") == 0)
    module = "";

  if (strcmp(module, "__TRAIN__") == 0) {
    generateProduction = kTRUE;
    module = "";
  }

  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");

  TString dataFolder("./");

  Printf("Connecting to AliEn...");
  TGrid::Connect("alien:");

  Printf(">>>>>>> Reading train configuration ...");
  TObjArray *arr = AliAnalysisTaskCfg::ExtractModulesFrom("MLTrainDefinition.cfg");
  Printf(">>>>>>> Train configuration read");

  arr->Print();

  AliAnalysisAlien *plugin = new AliAnalysisAlien(train_name);
  // General plugin settings here
  plugin->SetProductionMode();

  plugin->SetAPIVersion("V1.1x");

  // libraries because we start with root!
  const char *rootLibs = "libVMC.so libPhysics.so libTree.so libMinuit.so libProof.so libSTEERBase.so libESD.so libAOD.so";
  plugin->SetAdditionalRootLibs(rootLibs);

  TString alirootVersion(gSystem->Getenv("ALIROOT_VERSION"));
  if (alirootVersion.Length() == 0)
  {
    Printf("ERROR: ALIROOT_VERSION not set. Did you source env.sh? Exiting...");
    return;
  }

  plugin->SetJobTag("test/test");
  plugin->SetROOTVersion(gSystem->Getenv("ROOT_VERSION_SHORT"));
  plugin->SetAliROOTVersion(alirootVersion);

  plugin->SetMaxMergeFiles(maxMergeFiles);
  plugin->SetTTL(ttl);
  plugin->SetAnalysisMacro(Form("%s.C", train_name));
  plugin->SetValidationScript("validation.sh");

  plugin->SetRegisterExcludes(excludeFiles + " AliAOD.root");
  if (friendChainNames.Length() > 0) {
    if (friendChainLibraries.Length() > 0)
      plugin->SetFriendChainName(friendChainNames, friendChainLibraries);
    else
      plugin->SetFriendChainName(friendChainNames);
  } else if (friendChainLibraries.Length() > 0) {
    Printf("friendChainLibraries: %s", friendChainLibraries.Data());
    plugin->SetAdditionalRootLibs(Form("%s %s", rootLibs, friendChainLibraries.Data()));
  }


  ///
  plugin->SetAdditionalLibs("AliAnalysisTaskMyTask.cxx AliAnalysisTaskMyTask.h");
  plugin->SetAnalysisSource("AliAnalysisTaskMyTask.cxx");
  ///

  // jemalloc
  additionalpackages += " jemalloc::latest";
  plugin->AddExternalPackage(additionalpackages);

  plugin->SetJDLName(Form("%s.jdl", train_name));
  plugin->SetExecutable(Form("%s.sh", train_name));
  plugin->SetSplitMode("se");

  plugin->SetGridOutputDir("./");
  plugin->SetGridWorkingDir("./");

  if (!generateProduction)
    plugin->SetKeepLogs(kTRUE);

  plugin->SetMergeViaJDL();

  if (dataFolder.Length() == 0) {
    Printf("ERROR: TRAIN_TESTDATA not set. Exiting...");
    return;
  }

  if (dataBasePath_array[0].Length() > 0 || index>1) {
    TString archiveName = "";
    if (friendChainNames.Length() > 0) {
	    friendChainNames = friendChainNames.ReplaceAll(" ", ";");
	  }

	  if(index>1)
	    plugin->SetNtestFiles((index-1)*nFiles);
	  else
	    plugin->SetNtestFiles(nFiles);

    // Is MC only?
    if (iAOD == 3) {
      Printf(">>>> Expecting MC only production");
      plugin->SetUseMCchain();
    }

	  TString meta_fileName;
	  meta_fileName.Form("__alice__meta__input__%d_%s.txt", trainId, periodName.Data());
	  std::ofstream outfile (meta_fileName);

	  TString cdir = gSystem->WorkingDirectory();
	  gSystem->cd(dataFolder);

	  for(int meta_index=0; meta_index<index; meta_index++) {
	    //if no meta dataset
	    if(index>1 && meta_index==0)
	      continue;//meta datasets are filled for meta_index>=1, standard dataset are filled for meta_index=0

	    TString dataTag;
	    dataTag.Form("%s_%s_%s_%d", dataBasePath_array[meta_index].Data(), archiveName.Data(), dataAnchor.Data(), nFiles);

	    dataTag.ReplaceAll(";", "__");
	    dataTag.ReplaceAll("/", "__");
	    dataTag.ReplaceAll(".root", "");
	    dataTag.ReplaceAll(".zip", "");
      dataTag.ReplaceAll(".", "_");

	    TString dataFileName(dataTag + ".txt");

	    Printf("\n>>>> Test files are taken from: %s", dataFileName.Data());
	    if(index==1)
	      plugin->SetFileForTestMode(Form("%s/%s", dataFolder.Data(), dataFileName.Data()));
	    else
	      plugin->SetFileForTestMode(Form("%s/%s", dataFolder.Data(), meta_fileName.Data()));

	    // Copy dataset locally
      Bool_t copyData = true;
	    if (copyData) {
	      // check if files are already copied
        if (gSystem->AccessPathName(dataFileName)) {
		      if (!gGrid) {
		        Printf("Connecting to AliEn...");
		        TGrid::Connect("alien:");
		      }

		      // special treatment for non-officially produced productions
		      Bool_t specialSet = kFALSE;
		      if (periodName == "AMPT_LHC12g6" || periodName == "AMPT_LHC12c3")
            specialSet = kTRUE;

		      Bool_t result = kFALSE;
          unsigned int oldTimeOut = TFile::SetOpenTimeout(5);

          TString data_files = dataAnchor;

		      if (specialSet) {
            if (!data_files.IsNull()) data_files += ";";
		        data_files += "Kinematics.root";
		      } else {
            //for MC datasets try to download Kinematics.root and galice.root if they are available
            if(((iAOD == 0) || (iAOD == 3)/*|| (iAOD == 1) || (iAOD == 2)*/)
                && (TString(dataBasePath_array[meta_index].Data()).Index("sim")>0)) {
              gSystem->Load("libpythia6.so");//library so that AliGenPythia in galice.root can be loaded, not all MC train runs have this library loaded
              if (!data_files.IsNull()) data_files += ";";
              data_files += "galice.root;Kinematics.root";
              //check if is this a pT hard dataset
              //the regular expressions do not work with {1,2} quantifier
              if(TString(dataBasePath_array[meta_index]+"/").Index(TRegexp("/[0-9][0-9]/"))>0||TString(dataBasePath_array[meta_index]+"/").Index(TRegexp("/[0-9]/"))>0) {
                if(iAOD == 0)
                  data_files += ";pyxsec.root";
                else if (iAOD == 1 || iAOD == 2)
                  data_files += ";pyxsec_hists.root";
              }
            }

            if (friendChainNames.Length() > 0) {
              if (!data_files.IsNull()) data_files += ";";
              data_files += friendChainNames;
            }

          }
          result = CopyLocalDataset(dataBasePath_array[meta_index].Data(), data_files, nFiles, dataFileName, archiveName, dataTag);
          TFile::SetOpenTimeout(oldTimeOut);

  		    if (!result) {
		        gSystem->Unlink(dataFileName);
		        gSystem->Exec(Form("rm -rf %s", dataTag.Data()));
		        Printf("ERROR: Could not copy test files. Exiting...");
		        return;
		      }
	      } else {
		      // mark files as used (for cleanup)
		      gSystem->Exec(Form("touch %s", dataFileName.Data()));
	      }
      }
  	  if(index>1) {
	      //read input files paths for this subdataset and add it to the meta dataset
	      string line;
        ifstream myfile (Form("%s/%s", dataFolder.Data(), dataFileName.Data()));
        if (myfile.is_open()) {
	        while ( getline (myfile,line) ) {
		        outfile << line << std::endl;
		      }
	        myfile.close();
	      }
	    }
    }
  	outfile.close();
	  gSystem->cd(cdir);
  }
  // execute custom configuration
  Int_t error = 0;
  gROOT->Macro("globalvariables.C", &error);
  if (error != 0) {
    Printf("ERROR: globalvariables.C was not executed successfully...");
    return;
  }

  // Load modules here
  plugin->AddModules(arr);
  plugin->CreateAnalysisManager("train", "handlers.C");

  // specific if data is processed or MC is generated on the fly
  if (iAOD == 100) { // MC production
    Long64_t totalEvents = TString(gSystem->Getenv("GEN_TOTAL_EVENTS")).Atoll();

    //Printf("%lld %d", totalEvents, splitMaxInputFileNumber);

    Long64_t neededJobs = totalEvents / splitMaxInputFileNumber;

    plugin->SetMCLoop(true);
    plugin->SetSplitMode(Form("production:1-%lld", neededJobs));
    plugin->SetNMCjobs(neededJobs);
    plugin->SetNMCevents((generateProduction) ? splitMaxInputFileNumber : nTestEvents);
    plugin->SetExecutableCommand("aliroot -b -q");

    //plugin->AddDataFile("dummy.xml"); // TODO to be removed
    // /alice/cern.ch/user/a/alitrain/

    plugin->SetKeepLogs(kTRUE);
  } else { // Data, ESD/AOD
    plugin->SetSplitMaxInputFileNumber(splitMaxInputFileNumber);

    //plugin->AddDataFile(Form("$1/%s/lego_train_input.xml", trainFolder));

    plugin->SetExecutableCommand("root -b -q");
    plugin->SetInputFormat("xml-single");
  }

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  ///
  mgr->SetUseProgressBar(1, 25);
  ///
  mgr->SetDebugLevel(debugLevel);
  mgr->SetNSysInfo((strcmp(gSystem->Getenv("PP"), "true") == 0) ? 1000 : 40);

  mgr->SetFileInfoLog("fileinfo.log");

  if (generateProduction)
    plugin->GenerateTrain(train_name);
  else
    plugin->GenerateTest(train_name, module);

  // check for illegally defined output files
  TString validOutputFiles = gSystem->Getenv("OUTPUT_FILES");
  validOutputFiles += "," + excludeFiles;
  TString outputFiles = plugin->GetListOfFiles("out");
  TObjArray *output_tokens = outputFiles.Tokenize(",");

  Bool_t valid = kTRUE;
  for (Int_t i=0; i<output_tokens->GetEntries(); i++)
  {
    if (!validOutputFiles.Contains(output_tokens->At(i)->GetName()))
    {
      Printf("ERROR: Output file %s requested which is not among the defined ones for this train (%s)", output_tokens->At(i)->GetName(), validOutputFiles.Data());
      valid = kFALSE;
    }
  }
  delete output_tokens;

  if (!valid) {
    Printf(">>>>>>>>> Invalid output files requested. <<<<<<<<<<<<");
    gSystem->Unlink("lego_train.C");
  }
}

Int_t CopyLocalDataset(const char *griddir, const char *pattern, Int_t nfiles, const char *output, const char *archivefile, const char *outputdir)
{
// Copy data from the given grid directory according a pattern and make a local
// dataset.
// archivefile (optional) results in that the archive containing the file <pattern> is copied. archivefile can contain a list of files (semicolon-separated) which are all copied
   Printf("CopyLocalDataset requesting files: %s", pattern);
   if (!gGrid && !TGrid::Connect("alien:")) {
      Printf("### Error: CopyLocalDataset : Cannot copy local dataset with no grid connection");
      return 0;
   }
   if (!AliAnalysisAlien::DirectoryExists(griddir)) {
      Printf("### Error: CopyLocalDataset : Data directory %s not existing.", griddir);
      return 0;
   }
   TObjArray *download_files = 0;
   TString pattern_anchor = pattern;
   if (strlen(pattern) > 0 && TString(pattern).Contains(";")) {
      download_files = TString(pattern).Tokenize(";");
      pattern_anchor = download_files->At(0)->GetName();
      download_files->RemoveAt(0);
      download_files->Compress();
   }

   TString command = Form("find -l %d %s %s", nfiles+10, griddir, pattern_anchor.Data());
   printf("=== Running command: %s\n", command.Data());
   TGridResult *res = gGrid->Command(command);
   Int_t nfound = res->GetEntries();
   if (!nfound) {
      Error("CopyLocalDataset", "No file found in <%s> having pattern <%s>", griddir, pattern_anchor.Data());
      return 0;
   }
   printf("=== Copying locally %d files ...\n", nfiles);

   // archives
   TObjArray* additionalArchives = 0;
   if (strlen(archivefile) > 0 && TString(archivefile).Contains(";")) {
      additionalArchives = TString(archivefile).Tokenize(";");
      archivefile = additionalArchives->At(0)->GetName();
      additionalArchives->RemoveAt(0);
      additionalArchives->Compress();
   }

   // Copy files locally
   ofstream out;
   out.open(output, ios::out);
   TMap *map;
   TString turl, dirname, filename, temp;
   TString cdir = gSystem->WorkingDirectory();
   gSystem->MakeDirectory(outputdir);
   gSystem->ChangeDirectory(outputdir);
   Int_t ncopied = 0;
   for (Int_t i=0; i<nfound; i++) {
      if (ncopied == nfiles) break;
      map = (TMap*)res->At(i);
      turl = map->GetValue("turl")->GetName();
      filename = gSystem->BaseName(turl);
      dirname = gSystem->DirName(turl);
      dirname = gSystem->BaseName(dirname);
      gSystem->MakeDirectory(dirname);

      TString source(turl);
      TString targetFileName(filename);
      TString target;

      if (strlen(archivefile) > 0) {
// TODO here the archive in which the file resides should be determined
// however whereis returns only a guid, and guid2lfn does not work
// Therefore we use the one provided as argument for now
         source = Form("%s/%s", gSystem->DirName(source.Data()), archivefile);
         targetFileName = archivefile;
      }
      target.Form("./%s/%s", dirname.Data(), targetFileName.Data());
      Printf("=> Downloading %s ...", source.Data());
      if (!TFile::Cp(source, Form("file:%s", target.Data()))
          || gSystem->AccessPathName(target)) {
        Printf("### skipping files from %s since anchor file cannot be opened", dirname.Data());
        gSystem->Exec(Form("rm -rf %s", dirname.Data()));
        continue;
      }
      Bool_t success = kTRUE;
      if (additionalArchives) {
        for (Int_t j=0; j<additionalArchives->GetEntriesFast(); j++) {
          target.Form("./%s/%s", dirname.Data(), additionalArchives->At(j)->GetName());
          gSystem->MakeDirectory(gSystem->DirName(target));
          Printf("=> Downloading %s ...", additionalArchives->At(j)->GetName());
          success &= TFile::Cp(Form("%s/%s", gSystem->DirName(source.Data()), additionalArchives->At(j)->GetName()), Form("file:%s", target.Data()))
                     && !gSystem->AccessPathName(target);
          if (!success) {
            Printf("   ### file: %s not found or could not be copied", additionalArchives->At(j)->GetName());
            break;
          }
        }
      }

      // Copy the additional files
      if (success && download_files && strlen(archivefile) == 0) {
        for (Int_t j=0; j<download_files->GetEntriesFast(); j++) {
          source = turl;
          source = Form("%s/%s", gSystem->DirName(source.Data()), download_files->At(j)->GetName());
          target.Form("./%s/%s", dirname.Data(), download_files->At(j)->GetName());
          gSystem->MakeDirectory(gSystem->DirName(target));
          Printf("=> Downloading %s ...", source.Data());
          success = TFile::Cp(source, Form("file:%s", target.Data())) && !gSystem->AccessPathName(target);
          if (!success) {
            Printf("   ### file: %s not found or could not be copied", source.Data());
            break;
          } else {
            Printf("=> file: %s copied successfully", source.Data());
          }
        }
      }

      if (success) {
        if (strlen(archivefile) > 0) targetFileName = Form("%s#%s", targetFileName.Data(), gSystem->BaseName(turl.Data()));
        out << cdir << Form("/%s/%s/%s", outputdir, dirname.Data(), targetFileName.Data()) << endl;
        ncopied++;
      } else {
        printf("### skipping files from %s since some files cannot be opened\n", dirname.Data());
        gSystem->Exec(Form("rm -rf %s", dirname.Data()));
      }
   }
   gSystem->ChangeDirectory(cdir);
   delete res;
   delete additionalArchives;
   delete download_files;
   return ncopied;
}
