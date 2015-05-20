#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Port.h>
#include <yarp/os/all.h>


#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/all.h>
#include <yarp/math/Math.h>


#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>


#include <gsl/gsl_math.h>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <sstream>
#include <boost/filesystem.hpp>

#include <yarp/dev/ControlBoardInterfaces.h>

#include <yarp/dev/IControlMode2.h>



YARP_DECLARE_DEVICES(icubmod)

using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;


class CtrlThread: public RateThread
{
    protected:
        std::vector<PolyDriver*>        robotDevice;
        std::vector<IEncoders*>         iEnc;
        std::vector<IPositionControl*>  iPos;
        std::vector<IVelocityControl*>  iVel;
        std::vector<ITorqueControl*>    iTrq;
        std::vector<IControlLimits*>    iLims;



        std::vector<Vector>     encoders;
        std::vector<Vector>     command;
        std::vector<Vector>     tmp;
        std::vector<Vector>     homeVectors;
        std::vector<Vector>     jointLimitsLower;
        std::vector<Vector>     jointLimitsUpper;

        std::vector<int> nJoints;

        yarp::dev::IControlMode2 *iMode;

        std::vector<std::string> robotParts;

        std::string currentRobotPart, robotName, baseFilePath, extension, pidLogFilePath;

        std::ofstream pidFile;
        int numRobotParts;

        bool isPositionMode, isVelocityMode, isTorqueMode, jointCommandsHaveBeenUpdated, controlThreadFinished;

        int partIndex, jointIndex;


        bool reverseDirectrion;
        int reversalCounter;





    public:
        CtrlThread(const double period);

        virtual bool threadInit();

        virtual void afterStart(bool s);

        virtual void run();

        virtual void threadRelease();

        bool openInterfaces();

        bool goToHome();
        bool sendJointCommands();
        void setCommandToHome();


        void createPidLog();

        void writeToPidLog();

        const std::string currentDateTime();

        bool isFinished();

    private:
        double Kp, Kd, Ki;
        void sendPidGains();
        void parseIncomingGains(Bottle *newGainMessage);
        void updatePidInformation();

        BufferedPort<Bottle>    gainsBufPort_in; // incoming new gains
        Port                    gainsPort_out; // outgoing current gains

        BufferedPort<Bottle>    goToHomeBufPort_in; // incoming gotToHome Command

        BufferedPort<Bottle>    robotPartAndJointBufPort_in;

};