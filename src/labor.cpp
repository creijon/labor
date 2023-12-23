#include <tclap/CmdLine.h>

#include <taskpool.h>

#include <chrono>
#include <thread>

static bool PrintPath(const std::string& path)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << path << std::endl;
    return true;
}

static void Complete()
{
    std::cout << "Tasks Complete" << std::endl;
}

int main(int argc, char *argv[])
{
    std::string inputPath;
    std::string outputPath;

	try 
    {  
        TCLAP::CmdLine cmd("Audio File Converter");
        TCLAP::ValueArg<std::string> inputPathArg("i", "inputPath", "Input File Path", true, "", "string");
        cmd.add(inputPathArg);
        TCLAP::ValueArg<std::string> outputPathArg("o", "outputPath", "Output File Path", true, "", "string");
        //cmd.add(outputPathArg);
        cmd.parse(argc,argv);

	    inputPath = inputPathArg.getValue();
	    //outputPath = outputPathArg.getValue();
    }
    catch (TCLAP::ArgException &e)
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
        return 1;
    }

    if (inputPath.empty())
    {
        std::cerr << "Could not open input file: " << inputPath << std::endl;
        return 1;
    }

    std::vector<std::string> paths = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten"};

    TaskPool pool(paths, PrintPath, Complete);

    // Not strictly necessary, since all worker threads are waited on in the destructor of the TaskPool. 
    // But this enables us to set a breakpoint.
    pool.WaitToComplete();

    return 0;
}
