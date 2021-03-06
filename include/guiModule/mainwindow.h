/*!
*  \file       mainwindow.h
*  \brief      MainWindow class header. This file declares the
               pidTunerGui module which communicates with pidTunerController
               and allows the user to modify PIDs and visualize their
               performance.
*  \author     Ryan Lober
*  \email      ryan.lober@isir.upmc.com
*  \version    1.0
*  \date       28 May 2015
*  \copyright  GNU Public License.
*
* This file is part of pidTuner, a set of modules for manual
* tuning of individual joint low level gains on the iCub robot.
* For more details please look at the README or check out the
* Github repository: https://github.com/rlober/pidTuner.git
*
* pidTuner is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* pidTuner is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with pidTuner.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLineEdit>
#include <yarp/os/all.h>

#include <boost/scoped_ptr.hpp>

#include <tinyxml.h>

#include "qcustomplot.h"

#include <MessageVocabulary.h>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include <yarp/sig/Vector.h>
#include <yarp/math/Math.h>

#include <GenericPid.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool init(yarp::os::ResourceFinder &rf);
    void initializeGui();

private slots:
    void createPlot();
    void on_gainTestButton_clicked(bool checked);
    void addPartsToList();
    void on_partList_currentIndexChanged(int partId);
    void on_jointList_currentIndexChanged(int jointId);
    void on_closeButton_clicked();
    void on_homeButton_clicked();
    void on_nextJointButton_clicked();
    void on_previousJointButton_clicked();
    void on_posContButton_clicked(bool checked);
    void on_velContButton_clicked(bool checked);
    void on_torContButton_clicked(bool checked);
    void on_kd_in_editingFinished();
    void on_kp_in_editingFinished();
    void on_ki_in_editingFinished();
    void on_kff_in_editingFinished();
    void on_max_int_in_editingFinished();
    void on_scale_in_editingFinished();
    void on_max_output_in_editingFinished();
    void on_offset_in_editingFinished();
    void on_stiction_up_in_editingFinished();
    void on_stiction_down_in_editingFinished();
    void on_bemf_in_editingFinished();
    void on_bemf_scale_in_editingFinished();
    void on_ktau_in_editingFinished();
    void on_ktau_scale_in_editingFinished();
    void on_coulombVelThresh_in_editingFinished();
    void on_frictionCompensation_in_editingFinished();
    // void on_frictionCompensation_in_textChanged(QString& newText);


    void on_saveGainsButton_clicked();
    void on_gainResetButton_clicked();

    void on_amplitude_in_editingFinished();
    void on_duration_in_editingFinished();
    void on_startTime_in_editingFinished();
    void on_resetSignalPropButton_clicked();
    void on_savePlotButton_clicked();


    void frictionCompensationIntSlot(int value);
    void frictionCompensationSliderTextSlot(QString newText);


private:
    boost::scoped_ptr<Ui::MainWindow> ui;
    yarp::os::Log log;

    //Functions
    bool initialize(yarp::os::ResourceFinder &rf);
    void resetYLabel();
    void setCurrentPartAndJoint();
    bool discardChanges();
    void saveGains();
    void refreshGainDisplays();
    bool getPidValues();
    bool setPidValues(bool testNewPid=true);
    void sendPartAndJointIndexes();
    void sendControlMode();
    void writeDataToLogs();
    void createDataLogs();
    const std::string currentDateTime();
    void sendExcitationSignalProperties();
    void updateSignalPropertiesInGui();
    void setSignalPropertiesToDefaults();
    double getValueFromUserInput(QLineEdit* userInputBox);
    void disableTorqueParamInputs();
    void enableTorqueParamInputs();
    bool checkIfUsingJtc();



    //Variables
    QString yPlotLabel;
    boost::scoped_ptr<QCPPlotTitle> plotTitle;

    bool isOnlyMajorJoints, gainsHaveBeenChanged;
    int partIndex, jointIndex;

    bool initFinished;
    std::vector<std::string> partsListVector;
    bool doExcludePart;
    int excludedPartIndex;

    bool usingJTC;

    double  signalAmplitude,
            signalAmplitude_POS,
            signalAmplitude_VEL,
            signalAmplitude_TOR,
            signalAmplitude_POS_DEFAULT,
            signalAmplitude_VEL_DEFAULT,
            signalAmplitude_TOR_DEFAULT;
    double  signalStartTime,
            signalStartTime_POS,
            signalStartTime_VEL,
            signalStartTime_TOR,
            signalStartTime_POS_DEFAULT,
            signalStartTime_VEL_DEFAULT,
            signalStartTime_TOR_DEFAULT;

    double  signalDuration,
            signalDuration_POS,
            signalDuration_VEL,
            signalDuration_TOR,
            signalDuration_POS_DEFAULT,
            signalDuration_VEL_DEFAULT,
            signalDuration_TOR_DEFAULT;

    yarp::os::Network   yarp;
    yarp::os::Port      dataPort_in;
    yarp::os::RpcClient rpcClientPort;

    ControlMode testControlMode;
    SignalType  testSignalType;

    std::string controlMode_string;
    std::string logFilePath;

    GenericPid newPid, originalPid;

    static const int FRICTION_COMPENSATION_SLIDER_MAX = 1000;
    static const int FRICTION_COMPENSATION_SLIDER_MIN = 0;


};

#endif // MAINWINDOW_H
