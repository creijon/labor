#include <tclap/CmdLine.h>

#include <taskpool.h>

#include <chrono>
#include <thread>

class PathTask : public Task
{
public:
    PathTask(const std::string& str)
        : _path(str)
    {
    }

    void operator()() override
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << _path << std::endl;
    }

private:
    std::string _path;
};

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

    std::vector<std::shared_ptr<Task>> paths =
        { std::make_shared<PathTask>("One"),
          std::make_shared<PathTask>("Two"),
          std::make_shared<PathTask>("Three"),
          std::make_shared<PathTask>("Four"),
          std::make_shared<PathTask>("Five"),
          std::make_shared<PathTask>("Six"),
          std::make_shared<PathTask>("Seven"),
          std::make_shared<PathTask>("Eight"),
          std::make_shared<PathTask>("Nine"),
          std::make_shared<PathTask>("Ten") };

    TaskPool pool(paths, Complete);

    return 0;
}
