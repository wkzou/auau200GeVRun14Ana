#ifndef StPicoD0AnaMaker_h
#define StPicoD0AnaMaker_h

/* **************************************************
 *  A Maker to read a StPicoEvent and StPicoD0Event
 *  simultaneously and do analysis. 
 *
 *  Please write your analysis in the ::Make() function.
 *
 *  Authors:  Mustafa Mustafa (mmustafa@lbl.gov)
 *
 * **************************************************
 */

#include "TChain.h"
#include "StMaker.h"
#include "StThreeVectorF.hh"

class TString;
class TFile;
class TNtuple;
class StPicoD0Event;
class StKaonPion;
class StPicoTrack;
class StPicoDstMaker;
class StPicoD0AnaHists;
class StRefMultCorr;

class StPicoD0AnaMaker : public StMaker
{
  public:
    StPicoD0AnaMaker(char const * name, TString const inputFilesList, 
        TString const outBaseName,StPicoDstMaker* picoDstMaker, StRefMultCorr* grefmultCorrUtil);
    virtual ~StPicoD0AnaMaker();

    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();

    int getEntries() const;

  private:
    StPicoD0AnaMaker() {}
    void readNextEvent();

    int  getD0PtIndex(StKaonPion const* ) const;
    bool isGoodEvent(StPicoEvent const*) const;
    bool isGoodQaTrack(StPicoTrack const * ,StThreeVectorF const momentum ,const double dca) const;
    bool isGoodTrack(StPicoTrack const*, StKaonPion const*) const;
    bool isTpcPion(StPicoTrack const*) const;
    bool isTpcKaon(StPicoTrack const*) const;
    bool isTofPion(StPicoTrack const* const, float beta) const;
    bool isTofKaon(StPicoTrack const* const, float beta) const;
    bool isGoodPair(StKaonPion const*) const;
    float getTofBeta(StPicoTrack const*,StThreeVectorF const * pVtx) const;

    StPicoDstMaker* mPicoDstMaker;
    StPicoD0Event* mPicoD0Event;
    StRefMultCorr* mGRefMultCorrUtil;

    TString mInputFilesList;
    TString mOutFileBaseName;
    TChain* mChain;
    int mEventCounter;

    // -------------- USER variables -------------------------
    // add your member variables here. 
    // Remember that ntuples size can be really big, use histograms where appropriate
    StPicoD0AnaHists* mHists;

    ClassDef(StPicoD0AnaMaker, 1)
};

inline int StPicoD0AnaMaker::getEntries() const 
{
  return mChain? mChain->GetEntries() : 0;
}

inline void StPicoD0AnaMaker::readNextEvent()
{
  mChain->GetEntry(mEventCounter++);
}
#endif
