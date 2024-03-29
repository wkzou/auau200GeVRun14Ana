void runPicoD0AnaMaker(TString d0list, TString outFileName, TString badRunListFileName = "picoList_bad_MB.list")
{
  TStopwatch*   stopWatch = new TStopwatch();
  stopWatch->Start();
  //Check STAR Library. Please set SL_version to the original star library used in the production from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  string SL_version = "SL15c";
  string env_SL = getenv("STAR");
  if (env_SL.find(SL_version) == string::npos)
  {
    cout << "Environment Star Library does not match the requested library in runPicoD0EventMaker.C. Exiting..." << endl;
    exit(1);
  }

  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();

  gSystem->Load("StPicoDstMaker");
  gSystem->Load("StPicoPrescales");
  gSystem->Load("StPicoD0EventMaker");
  gSystem->Load("StPicoD0AnaMaker");
  gSystem->Load("StRefMultCorr");
  // gSystem->Load("StEventPlane");

  chain = new StChain();

  // create list of picoDst files
  TString command = "sed 's/hft\\\/d0tree/picodsts/g' " + d0list + " >correspondingPico.list";
  gSystem->Exec(command.Data());
  command = "sed -i 's/picoD0/picoDst/g' correspondingPico.list";
  gSystem->Exec(command.Data());
  StPicoDstMaker* picoDstMaker = new StPicoDstMaker(0, "correspondingPico.list", "picoDstMaker");
  StRefMultCorr* grefmultCorrUtil  = CentralityMaker::instance()->getgRefMultCorr();
  // StEventPlane*  eventPlaneMaker = new StEventPlane("eventPlaneMaker",picoDstMaker,grefmultCorrUtil);
  // StPicoD0AnaMaker*  picoD0AnaMaker = new StPicoD0AnaMaker("picoD0AnaMaker", d0list, outFileName.Data(), picoDstMaker, grefmultCorrUtil, eventPlaneMaker);
  StPicoD0AnaMaker*  picoD0AnaMaker = new StPicoD0AnaMaker("picoD0AnaMaker", d0list, outFileName.Data(), picoDstMaker, grefmultCorrUtil);
  grefmultCorrUtil->setVzForWeight(6, -6.0, 6.0);
  grefmultCorrUtil->readScaleForWeight("StRoot/StRefMultCorr/macros/weight_grefmult_vpd30_vpd5_Run14.txt");
  for(Int_t i=0;i<6;i++){
    cout << i << " " << grefmultCorrUtil->get(i, 0) << endl;
  }

  // -------------- USER variables -------------------------

  chain->Init();
  int nEntries = picoD0AnaMaker->getEntries();
  cout<<"Processing "<<nEntries<<" events..."<<endl;
  for (int iEvent = 0; iEvent < nEntries; ++iEvent)
  {
    chain->Clear();
    if(iEvent && iEvent%2000 == 0) cout<<"... finished processing "<<iEvent<<" events."<<endl;

    int iret = chain->Make();
    if (iret)
    {
      cout << "Bad return code!" << iret << endl;
      break;
    }
  }
  cout<<"Finished processing "<<nEntries<<" events."<<endl;

  chain->Finish();
  delete chain;

  // delete list of picos
  command = "rm -f correspondingPico.list";
  gSystem->Exec(command.Data());
  stopWatch->Stop();   
  stopWatch->Print();
}
