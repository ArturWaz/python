#include <iostream>
#include <windows.h>


#include <cmath>
#include <python.h>
#include <cstring>
#include <string>
#include <thread>
#include <mutex>


using namespace std;


class Plot {

    PyObject *obj_;
    FILE *pythonScriptFile_;
    std::string filename_;

    std::thread thread_;
    std::mutex mutex_;
    bool threaded_;

    double lastValue_;
    bool isUpdated_;

    static void thread(Plot *plot) {
        plot->pythonScriptFile_ == nullptr ? plot->initWithoutFile() : plot->initWithFile();
        double value = 0.0;
        uint8_t iter = 0;
        while (plot->threaded_) {

            plot->wait(0.01);

            ++iter;
            if (iter > 5) {
                plot->refreshPlot();
                iter = 0;
            }

            if (!plot->isUpdated_) continue;

            plot->mutex_.lock();
            value = plot->lastValue_;
            plot->isUpdated_ = false;
            plot->mutex_.unlock();

            plot->newValue(value);
        }
        plot->clear();
    }

protected:

    void initWithoutFile() {
        Py_Initialize();
        PyRun_SimpleString(
            ""
                    "import numpy as np\n"
                    "from matplotlib import pyplot as plt\n"

                    "x = [0.02*i-10 for i in range(500)]\n"
                    "y = [0]*500\n"

                    "plt.ion()\n"
                    "line, = plt.plot(x,y)\n"
                    "plt.xlim([min(x),max(x)])\n"
                    "plt.ylim([min(y)-1,max(y)+1])\n"

                    "def addValueToBuffer(i):\n"
                    "\ty.append(i)\n"
                    "\tdel y[0]\n"

                    "def refresh():\n"
                    "\tline.set_ydata(y)\n"

                    "def pause(i):\n"
                    "\tplt.pause(i)\n"
        );
    }

    inline void refreshPlot() {
        PyRun_SimpleString("refresh()");
    }

    inline void newValue(double const &value) {
        std::string function(std::string("addValueToBuffer(")+std::to_string(value)+std::string(")"));
        PyRun_SimpleString(function.c_str());
    }

    inline void wait(double const &timeInSecs) {
        std::string function(std::string("pause(")+std::to_string(timeInSecs)+std::string(")"));
        PyRun_SimpleString(function.c_str());
    }

    void initWithFile() {
        Py_Initialize();
        PyRun_SimpleFile(pythonScriptFile_, filename_.c_str());
    }
    void clear() {
        Py_Finalize();
    }

public:

    Plot(): pythonScriptFile_(nullptr), obj_(nullptr), threaded_(false) {}
    Plot(char const *filename): filename_(filename), pythonScriptFile_(nullptr), obj_(nullptr), threaded_(false) {
        obj_ = Py_BuildValue("s", Plot::filename_.c_str());
        pythonScriptFile_ = _Py_fopen_obj(obj_, "r+");
        if (pythonScriptFile_ == nullptr || filename == nullptr || obj_ == nullptr) throw "error";
    }
    ~Plot() {
        if (threaded_) close();
        if (pythonScriptFile_ != nullptr); // todo clean value
        if (obj_ != nullptr); // todo clean value
    }

    inline void addValue(double const &v) {
        mutex_.lock();
        isUpdated_ = true;
        lastValue_ = v;
        mutex_.unlock();
    }

    inline void show() {
        threaded_ = true;
        thread_ = std::thread(thread,this);
    }

    inline void close() {
        threaded_ = false;
        thread_.join();
    }

};




int main() {

    Plot plot;

    plot.show();

    for (int j = 0; j < 300; ++j) {
        plot.addValue(double(j)/10);
        Sleep(100);
    }

    return 0;
}