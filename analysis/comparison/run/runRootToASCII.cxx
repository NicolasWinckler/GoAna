/* 
 * File:   runRootToASCII.cxx
 * Author: winckler
 *
 * Created on May 5, 2015, 6:36 PM
 */

// std
#include <cstdlib>
#include <string>
#include <vector>

// root
#include "TApplication.h"
#include "TH1D.h"
#include "TCanvas.h"

// sids/goana lib
#include "SIDSLog.h"
#include "EsrInjData.h"
#include "EsrTree.h"
#include "SimpleConfig.h"
#include "SIDSFileManager.h"

/*
 * 
 */

// declare helper function
void PrintToFile(const std::string& filename, const std::vector<EsrInjData>& Container, const std::string& formatflag="decay-sorted");

// main function
int main(int argc, char** argv) 
{
    SET_LOG_LEVEL(DEBUG);
    
    SimpleConfig* config = nullptr; 
    SIDSFileManager* man = nullptr;
    std::vector<EsrInjData> DataContainer;
    
    try
    {
        config= new SimpleConfig();
        // add option to command line
        po::options_description formatDesc("Formatting options");
        formatDesc.add_options() 
            ("FormatFlag", po::value<std::string>()->default_value("decay-sorted"), "Flag for the text formatting of the ASCII output. Possible flags : \n"
                                                 "  decay-sorted \n"                                                   
                                                 "  decay-summary");
        config->AddToCmdLineOptions(formatDesc);
        
        if (config->ParseAll(argc,argv,true))
            return 0;
        
        // get command line input
        std::string filename=config->GetPar<std::string>("input.data.file.name");
        std::string treename=config->GetPar<std::string>("input.data.file.tree.name");
        std::string branchname=config->GetPar<std::string>("input.data.file.branch.name");
        std::string formatflag=config->GetPar<std::string>("FormatFlag");
        std::string outfilename=config->GetPar<std::string>("output.data.file.name");
        
        if(formatflag!="decay-sorted" && formatflag!="decay-summary")
        {
            SLOG(ERROR) <<"Format Flag "<<formatflag <<" not supported";
            SLOG(INFO)<< formatDesc << std::endl;
            return 1;
        }
       
            // open root file with file manager
            man = new SIDSFileManager(filename,treename,branchname);
            
            // get all (or pre-sorted) data from root file
            // DataContainer=man->GetAllData(true);
            // true = pre-sorted 
            // false = direct from tree
            
            if(formatflag=="decay-sorted")
                DataContainer=man->GetAllData(true);
            
            if(formatflag=="decay-summary")
                DataContainer=man->GetAllData(false);
            
            if(DataContainer.size()==0)
            {
                SLOG(ERROR)<<"No data found";
            }
            
            // print data to txt file
            std::ofstream txtfile(outfilename);

            if (txtfile.is_open())
            {
                // to print only decays that are pre-sorted
                if(formatflag=="decay-sorted")
                {
                    for(const auto& injection : DataContainer)
                    {
                        std::vector<EsrDecayEvent> eventlist = injection.GetECData();
                        for(const auto& event : eventlist)
                            txtfile << event.GetDecayTime() << std::endl;
                    }
                }
                // to print more info including duplicated analysis
                if(formatflag=="decay-summary")
                {
                    for(const auto& injection : DataContainer)
                    {
                        txtfile << injection.GetFileName() << "   ";
                        std::vector<EsrDecayEvent> eventlist = injection.GetECData();
                        for(const auto& event : eventlist)
                        {
                            txtfile << event.GetDecayTime() << "   " 
                                    << event.GetDecayFreq() << "   ";
                        }
                        if(!injection.GetFileComment().empty())
                            txtfile << "("<< injection.GetFileComment() << ")";
                        txtfile << std::endl;
                    }
                }
                txtfile.close();
            }
            else
                SLOG(ERROR) << "Could not open file "<< outfilename;
    
    }
    catch (std::exception& e)
    {
        SLOG(ERROR) << e.what();
        return 1;
    }
    
    
    if(config)
        delete config;
    
    if(man)
        delete man;
    
   
    return 0;
}

