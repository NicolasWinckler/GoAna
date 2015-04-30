/* 
 * File:   SIDSFileManager.cxx
 * Author: winckler
 * 
 * Created on October 3, 2014, 10:14 PM
 */


#include "SIDSFileManager.h"


SIDSFileManager::SIDSFileManager() : fDirName()
{
    
}

SIDSFileManager::SIDSFileManager(   const std::string& filename, 
                                    const std::string& treename, 
                                    const std::string& branchname,
                                    const std::string &dirname) 
                                   :    fFileName(filename), 
                                        fTreeName(treename), 
                                        fBranchName(branchname), 
                                        fDirName(dirname)
{
    // copy data from root tree
    EsrTree *DecayTree = new EsrTree(fFileName,fTreeName,fBranchName);
    fDataList=DecayTree->GetEsrData();
    delete DecayTree;
    
    std::vector<std::string> analyzedFiles;
    for(unsigned int k(0); k<fDataList.size(); k++)
    {
        std::string temp=fDataList[k].GetFileName();
        analyzedFiles.push_back(temp);
    }
    
    SetDirectory(fDirName);// used only for traces analysis --> get input traces files dir
    SetInputList(analyzedFiles);// 
    GetNonAnalyzedFiles();// used only for traces analysis --> look at directory fdirname content and check if trace files are not analyzed
    
    std::map<std::string,int> duplicates=GetDuplicatesList();
    
    
    // search for index of duplicates and fill non duplicates first
    for(unsigned int k(0);k<fDataList.size() ;k++)
    {
        std::string temp=fDataList[k].GetFileName();
        // if current file name at index k is in duplicates map
        // add the index to fDuplicatesIdx map
        if(duplicates.count(temp))
        {
            fDuplicatesIdx[temp].push_back(k);
        }
        else
        {
            // if it is not a duplicate store index directly to final index map
            fDataToPlotIdx[temp]=k;
        }
    }
    
    
    // count number of duplicates and add to final index map 
    // the missing duplicate key and the index corresponding 
    // to the last analyzed duplicated analysis 
    int sumduplicate=0;
    for(auto& p : fDuplicatesIdx)
    {
        sumduplicate+=p.second.size();
        fDataToPlotIdx[p.first]=p.second.back();
    }
    
    // fill sorted data
    for(auto& p : fDataToPlotIdx)
    {
        fSortedDataList.push_back(fDataList[p.second]);
    }
    
    
}

SIDSFileManager::SIDSFileManager(const std::string &dirname, const std::vector<std::string> &fileList)
{
    SetDirectory(dirname);
    SetInputList(fileList);
    GetNonAnalyzedFiles();
}

SIDSFileManager::~SIDSFileManager()
{
    
}


void SIDSFileManager::SetInputList(const std::vector<std::string> &fileList )
{
    fInputList=fileList;
    sort(fInputList.begin(), fInputList.end());
    CountDuplicates(fInputList);
}

int SIDSFileManager::SetDirectory(const std::string &dirname)
{
    fDirName=dirname;
    if(fDirFileList.size()>0)
        fDirFileList.clear();
    DIR *dir = opendir(fDirName.c_str());
    if(!dir)
    {
        return 1;
    }
    
    dirent *entry;
    while(entry = readdir(dir))
    {
        if(has_suffix(entry->d_name, ".root"))
        {
            //cout << entry->d_name << endl;
            fDirFileList.push_back(std::string(entry->d_name));
        }
    }

    closedir(dir);
    sort(fDirFileList.begin(), fDirFileList.end());
    return 0;
}


bool SIDSFileManager::has_suffix(const std::string& s, const std::string& suffix)
{
    return (s.size() >= suffix.size()) && equal(suffix.rbegin(), suffix.rend(), s.rbegin());    
}


void SIDSFileManager::CountDuplicates(const std::vector<std::string> &fileList)
{
    std::vector<std::string> inFileList=fileList;// copy and sort again in case it is called outside constructor
    sort(inFileList.begin(), inFileList.end());
    std::vector<std::string> outFileList (inFileList.size()); 
    std::vector<std::string>::iterator it;
    it=std::unique_copy (inFileList.begin(),inFileList.end(),outFileList.begin());
    outFileList.resize( std::distance(outFileList.begin(),it) );
    
    
    
    // for all unique file name
    for(auto& p : outFileList)
    {
        int countdupl=0;
        // for all analysis recorded
        for(auto& q : inFileList)
        {
            // if unique file name match all analysis file name
            if(p==q)
            {
                // count and update maps
                countdupl++;
                fAnalyzedFiles[p]=countdupl;
                if(countdupl>1)
                    fDuplicatesList[p]=countdupl;
            }
            
        }
    }
    
}

void SIDSFileManager::GetNonAnalyzedFiles()
{    
    for(auto p : fDirFileList)
    {
        if(fAnalyzedFiles.count(p)==0)
            fNonAnalyzedFiles.push_back(p);
    }
    
}


void SIDSFileManager::PrintAll(bool detail)
{
    SLOG(INFO)<<"*********************************";
    SLOG(INFO)<<"***** LIST OF ALL FILES *****";
    SLOG(INFO)<<"Root files found in directory: "<< fDirName;
    SLOG(INFO)<<" ";
    for(auto p : fDirFileList)
    {
        SLOG(INFO)<<p;
    }
    SLOG(INFO)<<" ";
    SLOG(INFO)<<"Number of files :"<<fDirFileList.size();
    SLOG(INFO)<<"*********************************";
}

void SIDSFileManager::PrintAnalyzed(bool detail)
{
    SLOG(INFO)<<"*********************************";
    SLOG(INFO)<<"***** LIST OF ALREADY ANALYZED FILES *****";
    SLOG(INFO)<<" ";
    for(auto p : fAnalyzedFiles)
    {
        if(detail)
            SLOG(INFO)<<p.second<< " analysis for file "<<p.first ;
        else
            SLOG(INFO)<<p.first ;
    }
    SLOG(INFO)<<" ";
    SLOG(INFO)<<"Number of files :"<<fAnalyzedFiles.size();
    SLOG(INFO)<<"*********************************";
}

void SIDSFileManager::PrintNotAnalyzed(bool detail)
{
    SLOG(INFO)<<"*********************************";
    SLOG(INFO)<<"***** LIST OF NOT YET ANALYZED FILES *****";
    SLOG(INFO)<<" ";
    for(auto p : fNonAnalyzedFiles)
    {
        SLOG(INFO)<<p;
    }
    SLOG(INFO)<<" ";
    SLOG(INFO)<<"Number of files :"<<fNonAnalyzedFiles.size();
    SLOG(INFO)<<"*********************************";
}


void SIDSFileManager::PrintDuplicates(bool detail, int anaNr)
{
    if(anaNr<1)
        anaNr=1;
    
    SLOG(INFO)<<"*********************************";
    SLOG(INFO)<<"***** LIST FILES ANALYZED MORE THAN "<< anaNr <<" TIMES *****";
    SLOG(INFO)<<" ";
    for(auto p : fDuplicatesList)
    {
        if(p.second>anaNr)
        {
            if(detail)
                SLOG(INFO)<<p.second<< " analysis for file "<<p.first ;
            else
                SLOG(INFO)<<p.first ;
        }
    }
    SLOG(INFO)<<" ";
    SLOG(INFO)<<"Number of files :"<<fDuplicatesList.size();
    SLOG(INFO)<<"*********************************";
    
}





