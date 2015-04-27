/* 
 * File:   SIDSProgOptions.cxx
 * Author: winckler
 * 
 * Created on March 11, 2015, 5:38 PM
 */

#include "SIDSProgOptions.h"

/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor / destructor
SIDSProgOptions::SIDSProgOptions(bool configfile) : 
                        fGenericDesc("Generic options description"), 
                        fConfigDesc("Configuration options description"), 
                        fHiddenDesc("Hidden options description"), 
                        fEnvironmentDesc("Environment Variables"),
                        fCmdline_options("Command line options"), 
                        fConfig_file_options("Configuration file options"), 
                        fVisible_options("Allowed options"),
                        fVerboseLvl(0), fConfigFile(), fHome_Env_Var()
{
    // //////////////////////////////////
    // define generic options
    fGenericDesc.add_options()
        ("help,h", "produce help")
        ("version,v", "print version")
        ("verbose", po::value<int>(&fVerboseLvl)->default_value(1), "Verbosity level")
        ;

    // TODO : change std::string fConfigFile to boost::optional type 
    if(configfile)
        fGenericDesc.add_options()
            ("config,c", po::value<std::string>(&fConfigFile)->required(), "Path to configuration file")
            ;

    // //////////////////////////////////
    // define environment variable description
    fEnvironmentDesc.add_options()
        ("HOME", po::value<boost::filesystem::path>(&fHome_Env_Var)->default_value(fHome_Env_Var),"Environment Variable: HOME")
        ;

    // //////////////////////////////////
    // define hidden options
    fHiddenDesc.add_options()
        ("input.dir.list", po::value< std::vector<std::string> >()->composing(), "input directory list")
        ("input.file.list", po::value< std::vector<std::string> >()->composing(), "input file list")
        ("output.dir.list", po::value< std::vector<std::string> >()->composing(), "output directory list")
        ("output.file.list", po::value< std::vector<std::string> >()->composing(), "output file list")
        ;

}

SIDSProgOptions::~SIDSProgOptions() 
{
}


/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// Add option descriptions

int SIDSProgOptions::AddToCmdLineOptions(const po::options_description& optdesc)
{
    fCmdline_options.add(optdesc);
    return 0;
}

int SIDSProgOptions::AddToCfgFileOptions(const po::options_description& optdesc)
{
    fConfig_file_options.add(optdesc);
    return 0;
}

int SIDSProgOptions::AddToVisibleOptions(const po::options_description& optdesc)
{
    fVisible_options.add(optdesc);
    return 0;
}

int SIDSProgOptions::AddToEnvironmentOptions(const po::options_description& optdesc)
{
    fEnvironmentDesc.add(optdesc);
    return 0;
}

/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// Parser

int SIDSProgOptions::ParseCmdLine(const int argc, char** argv, const po::options_description& desc, po::variables_map& varmap, bool AllowUnregistered)
{
    // //////////////////////////////////
    // get options from cmd line and store in variable map
    // here we use command_line_parser instead of parse_command_line, to allow unregistered and positional options
    if(AllowUnregistered)
    {
        po::command_line_parser parser{argc, argv};
        parser.options(desc).allow_unregistered();
        po::parsed_options parsedOptions = parser.run();
        po::store(parsedOptions,varmap);
    }
    else
        po::store(po::parse_command_line(argc, argv, desc), varmap);

    // //////////////////////////////////
    // call the virtual NotifySwitchOption method to handle switch options like e.g. "--help" or "--version"
    // return 1 if switch options found in varmap
    if(NotifySwitchOption())
        return 1;

    // "fix" options in varmap 
    po::notify(varmap);
    return 0;
}



int SIDSProgOptions::ParseCfgFile(const std::string& filename, const po::options_description& desc, po::variables_map& varmap, bool AllowUnregistered)
{
    std::ifstream ifs(filename.c_str());
    if (!ifs)
    {
        std::cout << "can not open configuration file: " << filename << "\n";
        return -1;
    }
    else
    {
        po:store(parse_config_file(ifs, desc, AllowUnregistered), varmap);
        po::notify(varmap);
    }
    return 0;
}

int SIDSProgOptions::ParseEnvironment(const std::function<std::string(std::string)> & environment_mapper)
{
    po::store(po::parse_environment(fEnvironmentDesc, environment_mapper), fvarmap);
    po::notify(fvarmap);
}


/*
int SIDSProgOptions::ParseAll(const int argc, char** argv, bool AllowUnregistered)
{
    // //////////////////////////////////
    // Method to overload.
    ParseCmdLine(argc,argv,fGenericDesc,fvarmap,AllowUnregistered);
    PrintOptions();

    return 0;
}
*/

/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// Print/notify options

int SIDSProgOptions::PrintHelp()
{
    std::cout << fVisible_options << "\n";
    return 0;
}

int SIDSProgOptions::PrintOptions()
{
    // //////////////////////////////////
    // Method to overload.
    // -> loop over variable map and print its content
    // -> In this example the following types are supported:
    // std::string, int, float, double, boost::filesystem::path
    // std::vector<std::string>, std::vector<int>, std::vector<float>, std::vector<double>


    MapVarValInfo_t mapinfo;

    // get string length for formatting and convert varmap values into string
    int maxlen_1st = 0;
    int maxlen_2nd = 0;
    int maxlen_TypeInfo = 0;
    int maxlen_default =0;
    int maxlen_empty = 0;
    int total_len=0;
    for (const auto& m : fvarmap)
    {
        Max(maxlen_1st, m.first.length());

        VarValInfo_t valinfo=Get_variable_value_info(m.second);
        mapinfo[m.first]=valinfo;
        std::string val_str;
        std::string typeInfo_str;
        std::string default_str;
        std::string empty_str;
        std::tie(val_str,typeInfo_str,default_str,empty_str)=valinfo;

        Max(maxlen_2nd, val_str.length());
        Max(maxlen_TypeInfo, typeInfo_str.length());
        Max(maxlen_default, default_str.length());
        Max(maxlen_empty, empty_str.length());

    }

    total_len=maxlen_1st+maxlen_2nd+maxlen_TypeInfo+maxlen_default+maxlen_empty;
    // formatting and printing

    SLOG(INFO)<<std::setfill ('*') << std::setw (total_len+3)<<"*";// +3 because of string " = "
    std::string PrintOptionsTitle="     Program options found     ";

    int leftSpace_len=0;
    int rightSpace_len=0;
    int leftTitle_shift_len=0;
    int rightTitle_shift_len=0;

    leftTitle_shift_len=PrintOptionsTitle.length()/2;

    if ((PrintOptionsTitle.length()) % 2)
        rightTitle_shift_len=leftTitle_shift_len+1;
    else
        rightTitle_shift_len=leftTitle_shift_len;

    leftSpace_len=(total_len+3)/2-leftTitle_shift_len;
    if ((total_len+3) % 2) 
        rightSpace_len=(total_len+3)/2-rightTitle_shift_len+1;
    else
        rightSpace_len=(total_len+3)/2-rightTitle_shift_len;


    SLOG(INFO) <<std::setfill ('*') << std::setw(leftSpace_len) <<"*"
                <<std::setw(PrintOptionsTitle.length()) << PrintOptionsTitle 
                <<std::setfill ('*') << std::setw(rightSpace_len) <<"*";

    SLOG(INFO) <<std::setfill ('*') << std::setw (total_len+3)<<"*";

    for (const auto& p : mapinfo)
    {
        std::string key_str;
        std::string val_str;
        std::string typeInfo_str;
        std::string default_str;
        std::string empty_str;
        key_str=p.first;
        std::tie(val_str,typeInfo_str,default_str,empty_str)=p.second;
        SLOG(INFO) << std::setw(maxlen_1st)<<std::left 
                    << p.first << " = " 
                    << std::setw(maxlen_2nd) 
                    << val_str 
                    << std::setw(maxlen_TypeInfo) 
                    << typeInfo_str 
                    << std::setw(maxlen_default)
                    << default_str 
                    << std::setw(maxlen_empty)
                    << empty_str;
    }
    SLOG(INFO)<<std::setfill ('*') << std::setw (total_len+3)<<"*";// +3 for " = "
    return 0;
}



int SIDSProgOptions::NotifySwitchOption()
{
    // //////////////////////////////////
    // Method to overload.
    if ( fvarmap.count("help") )
    {
        std::cout << "Possible Options : \n" << fVisible_options;
        return 1;
    }

    if (fvarmap.count("version")) 
    {
        std::cout << "alpha version 0.0\n";
        return 1;
    }

    return 0;
}

SIDSProgOptions::VarValInfo_t SIDSProgOptions::Get_variable_value_info(const po::variable_value& var_val)
    {
        // tuple<val_str, type_info_str, default_str, empty>
        auto& value = var_val.value();
        std::string defaulted_val;
        std::string empty_val;

        if(var_val.empty())
            empty_val="  [empty]";
        else
            if(var_val.defaulted())
                defaulted_val="  [default value]";
            else
                defaulted_val="  [provided value]";

        empty_val+=" *";
        // string
        if(auto q = boost::any_cast< std::string >(&value))
        {
            std::string val_str = *q;
            return std::make_tuple(val_str,std::string("  [Type=string]"),defaulted_val,empty_val);
        }

        // vector<string>
        if(auto q = boost::any_cast< std::vector<std::string> >(&value))
        {
            std::string val_str = variable_value_to_string< std::vector<std::string> >(var_val);
            return std::make_tuple(val_str,std::string("  [Type=vector<string>]"),defaulted_val,empty_val);
        }

        // int
        if(auto q = boost::any_cast< int >(&value))
        {
            std::string val_str =  variable_value_to_string< int >(var_val);
            return std::make_tuple(val_str,std::string("  [Type=int]"),defaulted_val,empty_val);
        }

        // vector<int>
        if(auto q = boost::any_cast< std::vector<int> >(&value))
        {
            std::string val_str = variable_value_to_string< std::vector<int> >(var_val);
            return std::make_tuple(val_str,std::string("  [Type=vector<int>]"),defaulted_val,empty_val);
        }

        // float
        if(auto q = boost::any_cast< float >(&value))
        {
            std::string val_str = variable_value_to_string< float >(var_val);
            return std::make_tuple(val_str,std::string("  [Type=float]"),defaulted_val,empty_val);
        }

        // vector<float>
        if(auto q = boost::any_cast< std::vector<float> >(&value))
        {
            std::string val_str = variable_value_to_string< std::vector<float> >(var_val);
            return std::make_tuple(val_str,std::string("  [Type=vector<float>]"),defaulted_val,empty_val);
        }

        // double
        if(auto q = boost::any_cast< double >(&value))
        {
            std::string val_str = variable_value_to_string< double >(var_val);
            return std::make_tuple(val_str,std::string("  [Type=double]"),defaulted_val,empty_val);
        }

        // vector<double>
        if(auto q = boost::any_cast< std::vector<double> >(&value))
        {
            std::string val_str = variable_value_to_string< std::vector<double> >(var_val);
            return std::make_tuple(val_str,std::string("  [Type=vector<double>]"),defaulted_val,empty_val);
        }

        // boost::filesystem::path
        if(auto q = boost::any_cast<boost::filesystem::path>(&value))
        {
            std::string val_str = (*q).string();
            return std::make_tuple(val_str,std::string("  [Type=boost::filesystem::path]"),defaulted_val,empty_val);
        }

        // if we get here, the type is not supported return unknown info
        return std::make_tuple(std::string("Unknown value"), std::string("  [Type=Unknown]"),defaulted_val,empty_val);
    }


