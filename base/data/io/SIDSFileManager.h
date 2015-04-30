/* 
 * File:   SIDSFileManager.h
 * Author: winckler
 *
 * Created on October 3, 2014, 10:14 PM
 */

#ifndef SIDSFILEMANAGER_H
#define	SIDSFILEMANAGER_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_set>
#include <iterator>
#include <algorithm>


#include "SIDSLog.h"
#include "dirent.h"
#include "EsrTree.h"
#include "EsrInjData.h"

using namespace std;

class show_copies {
    std::unordered_set<std::string> existing;
public:
    bool operator()(std::string const &in) {
        return existing.insert(in).second;
    }
};



class SIDSFileManager
{
public:
    enum
    {
        kPrintAll,
        kPrintAnalyzed,
        kPrintNotAnalyzed,
        kDuplicates,
        kDetail
    };
    
    SIDSFileManager();
    
    SIDSFileManager(const std::string& filename, const std::string& treename, const std::string& branchname, const std::string &dirname=""); 
    SIDSFileManager(const std::string &dirname, const std::vector<std::string> &fileList);
    virtual ~SIDSFileManager();
    
    void PrintAll(bool detail=false);
    void PrintAnalyzed(bool detail=false);
    void PrintNotAnalyzed(bool detail=false);
    void PrintDuplicates(bool detail=false, int anaNr=1);
    
    

    const std::vector<EsrInjData>& GetAllData(bool sorted=false)
    {
        if(sorted)
            return fSortedDataList;
        else
            return fDataList;
    }
    
    std::map<std::string,int> GetDuplicatesList()
    {
        return fDuplicatesList;
    }
    
protected:
    std::string fFileName; 
    std::string fTreeName;
    std::string fBranchName;
    std::string fDirName;
    std::vector<std::string> fDirFileList;
    std::vector<std::string> fInputList;
    std::map<std::string,int> fAnalyzedFiles;
    std::vector<std::string> fNonAnalyzedFiles;
    std::map<std::string,int> fDuplicatesList;
    
    std::vector<EsrInjData> fDataList;
    std::vector<EsrInjData> fSortedDataList;
    
    std::map<std::string,std::vector<int> > fDuplicatesIdx;
    std::map<std::string, int> fDataToPlotIdx;
    
    
    void SetInputList(const std::vector<std::string> &fileList );
    int SetDirectory(const std::string &dir);
    void GetNonAnalyzedFiles();
    bool has_suffix(const std::string& s, const std::string& suffix);
    void CountDuplicates(const std::vector<std::string> &fileList);
    
    
    
};

#endif	/* SIDSFILEMANAGER_H */

