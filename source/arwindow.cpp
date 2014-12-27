
// Precompiled header file
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <conio.h>

// faceAPI C++ wrapper classes
#include <sm_api_cxx.h>

// Qt
#pragma warning(disable : 4127)
#pragma warning(disable : 4512)


#include <QtOpenGL/QtOpenGL>
#include <QtCore/QtDebug>

// GLUT
#include <glut.h>

// STL
#include <set>
#include <algorithm>
#include "arwindow.h"

using namespace sm::faceapi::samplecode;
using namespace std;
#define UPDATE_EVENT_TYPE QEvent::User + 474;

namespace
{
    template <class ElemType>
    void setIdentity4x4(ElemType (&m)[16])
    {
        m[0] = 1.0f;
        m[1] = 0.0f;
        m[2] = 0.0f;
        m[3] = 0.0f;
        m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f;
        m[7] = 0.0f;
        m[8] = 0.0f;
        m[9] = 0.0f;
        m[10] = 1.0f;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }
}

ARWindow::ARWindow(const HeadPoseHandler &head_pose_handler, QWidget *parent, QGLWidget *share_widget): 
QGLWidget(parent, share_widget),
_head_pose_handler(head_pose_handler),
_image_painter(*this),
_mirror(true),
_clear_color(Qt::black)
{
    memset(&_head_pose_and_image.first,0,sizeof(_head_pose_and_image.first));
    setIdentity4x4(_ar_projection);
    setIdentity4x4(_ar_model_view);
    setIdentity4x4(_face_to_world);
	_start = false;
	_stop = false;

    // Render using a timer
    QTimer *timer = new QTimer(this);
    timer->setInterval(33);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start();
}

ARWindow::~ARWindow()
{
    makeCurrent();
}
/*help 
This slot is used for displaying contact information of developers involved in this project.
*/
void ARWindow::help()
{
		QMessageBox::about(this, tr("help menu"),
			tr("<p><TITLE>CSc 59867 - Senior Capstone Course II </TITLE><HR><H1>CSc 59867 <BR>Senior Capstone Course II </H1><HR><H2>Driver Distraction Detection</H2>You may be able to find answers to your questions online Qt Forums. Please feel free to contact us for assistance and suggestions, please call us or use the following contact persons for help with the suggestions and improvements to the  program. Be sure to use the email address on your account and enter your question in English in order for us to be able to respond to your query.&nbsp;<H2>Student</H2><IMG SRC='images/raj.jpg'ALIGN=LEFT><BR>Name:<b> Basavaraja Patri</b><BR>Student Computer Science Department CCNY NewYork<BR><TT><IMG SRC='images/send-email.gif'</IMG>:  <A HREF='mailto:bas1880@gmail.com'>bas1880@gmail.com</A><BR><b>Stay Connected: </b> <A HREF='https://www.facebook.com/bas1880'><IMG SRC='images/facebook.png'</IMG></A>  <A HREF='https://twitter.com/bas1880'><IMG SRC='images/twitter.gif'</IMG></A>  <A HREF='https://http://www.whatsapp.com/'><IMG SRC='images/whatsapp.jpg'</IMG></A>  <A HREF='https://www.youtube.com/'><IMG SRC='images/YouTube.png'</IMG></A></TT><H2>Instructor</H2> <IMG SRC='images/wolberg.jpg'  ALIGN=LEFT><BR>&nbsp;<BR>&nbsp;<BR>Professor <b>George Wolberg</b><BR>Office Hours: Wednesday, 11:30am-12:30pm, Room NAC 8/202I<BR><TT><IMG SRC='images/send-email.gif'</IMG>:  <A HREF='mailto:wolberg@cs.ccny.cuny.edu'>wolberg@cs.ccny.cuny.edu</A><BR><b>Contact Professor on college website: </b><A HREF='http://www-cs.engr.ccny.cuny.edu/~wolberg/'><IMG SRC='images/ccny.jpg'</IMG></A></TT><H2>Teaching Assistant</H2><b>Siavash Zokai</b><BR>Office Hours: By appointment<BR><TT><IMG SRC='images/send-email.gif'</IMG>:  <A HREF='mailto:siavash.zokai@gmail.com'>siavash.zokai@gmail.com</A></TT><HR><AUTHOR><I>Basavaraja Patri, December 13, 2014</AUTHOR></I></p>"));
}

/*
THIS SLOT IS USED FOR DISPLYING ABOUT-US DETAILS
*/
void ARWindow::about_us()
{
		QMessageBox::about(this, tr("About Us"),
			tr("<p><TITLE>CSc 59867 - Senior Capstone Course II </TITLE><HR><H1>CSc 59867 <BR>Senior Capstone Course II </H1><HR><H2>Driver Distraction Detection</H2><H2>Student</H2><BR><IMG SRC='images/raj.jpg' ALIGN=LEFT><BR>Name:<b> Basavaraja Patri</b><BR>Student Computer Science Department CCNY NewYork<BR><IMG SRC='images/send-email.gif'</IMG>:  <A HREF='mailto:bas1880@gmail.com'>bas1880@gmail.com</A><BR>Stay Connected:  <A HREF='https://www.facebook.com/bas1880'><IMG SRC='images/facebook.png'</IMG></A>  <A HREF='https://twitter.com/bas1880'><IMG SRC='images/twitter.gif'</IMG></A>  <A HREF='https://http://www.whatsapp.com/'><IMG SRC='images/whatsapp.jpg'</IMG></A>  <A HREF='https://www.youtube.com/'><IMG SRC='images/YouTube.png'</IMG></A><H2>Instructor</H2> <BR><IMG SRC='images/wolberg.jpg'  ALIGN=LEFT><BR>&nbsp;<BR>Professor <b>George Wolberg</b><BR>Office Hours: Wednesday, 11:30am-12:30pm, Room NAC 8/202I<BR><TT><IMG SRC='images/send-email.gif'</IMG>:  <A HREF='mailto:wolberg@cs.ccny.cuny.edu'>wolberg@cs.ccny.cuny.edu</A></BR><BR><b>Link to College WebSite: </b><A HREF='http://www-cs.engr.ccny.cuny.edu/~wolberg/'><IMG SRC='images/ccny.jpg'</IMG></A></TT><H2>Teaching Assistant</H2<BR><b>Siavash Zokai</b><BR>Office Hours: By appointment<BR><TT><IMG SRC='images/send-email.gif'</IMG>:  <A HREF='mailto:siavash.zokai@gmail.com'>siavash.zokai@gmail.com</A></TT><HR><AUTHOR><I>Basavaraja Patri, December 13, 2014</AUTHOR></I></p>"));
}

/*
THIS SLOT IS USED FOR DISPLYING ABOUT-PROJECT DETAILS
*/
void ARWindow::about_Project()
{
	QMessageBox::about(this, tr("About Project"),
		tr("<p><HR><H1>CSc 59867 <BR>Senior Capstone Course II </H1><HR><H2>Motivation</H2><b>          Saves Lives <ALIGN=CENTER></b><BR>We detect and prevent operator fatigue and distraction in real-time.<BR><IMG SRC='images/motivation.png'  ALIGN=LEFT></IMG>  <IMG SRC='images/distraction.png' ALIGN=RIGHT></IMG><H2>Project Details:</H2><BR>Distracted driving is any activity that could divert a person's attention away from the primary task of driving. All distractions endanger driver, passenger, and bystander safety. But, because text messaging requires visual, manual, and cognitive attention from the driver, it is by far the most alarming distraction.The best way to end distracted driving is to educate all Americans about the danger it poses.  Together, we can help save lives.<b> This project is a one of the potential solution for alrming the driver to avoid distraction by sending him alarm signals.</b><HR><H2>Project Development:</H2>This project IS developed based on Seingmachines FACEAPI free downloadable project. This Project developed in <b>C++ language</b>. Qt and OpenGL are used to perform GUI operations. In this project we used Microsoft's Visual Studio 2005, Qt 4.8.5 and OpenGL.<HR><H2>Special thanks to:</H2>Professor <b> George Wolberg, </b> teaching Assistant <b> Siavash Zokai</b>, and all <b>students </b>of this Senior design class for their <b>innovative ideas and thoughts to complete this project successfully.</b><BR><HR><AUTHOR><I>Basavaraja Patri, December 13, 2014</AUTHOR></I></p>"));
}
/*
This method displays the current version of the Qt Software used for this application in a message box
*/
void ARWindow::aboutQt()
{
		QMessageBox::about(this, tr("About Qt Version"),tr("Invoked<b>Help|aboutQt</b>"));	
}

//connect(aboutQt(), SIGNAL
QSize ARWindow::minimumSizeHint() const
{
    return QSize(640, 480);
}

//THIS constructor RETURNS QSize in array (640, 480)
QSize ARWindow::sizeHint() const
{
    return QSize(640, 480);
}

//This constructor returns mirror boolean value 
bool ARWindow::mirror() const
{
    return _mirror;
}
//sets boolean value
void ARWindow::setMirror(bool mirror)
{
    _mirror = mirror;
}
//bool ARWindow::start() const
//{
//	return _start;
//}
//void ARWindow::setStart(bool start)
//{
//	_start = start;
//}
//bool ARWindow::stop() const
//{
//	return _stop;
//}
//void ARWindow::setStop(bool stop)
//{
//	_stop = stop;
//}


void ARWindow::setClearColor(const QColor &color)
{
    _clear_color = color;
    updateGL();
}

void ARWindow::setARViewMatrices(GLfloat *model_view, GLfloat *projection)
{
    memcpy(&_ar_model_view[0],model_view,16*sizeof(GLfloat));
    memcpy(&_ar_projection[0],projection,16*sizeof(GLfloat));
}

void ARWindow::initializeGL()
{
    // Discover texture capabilities
    _image_painter.initializeGL();
}

void ARWindow::paintGL()
{	
		// Clear background
		glViewport(0, 0, QWidget::width(), QWidget::height());
		qglClearColor(_clear_color);
		glClear(GL_COLOR_BUFFER_BIT);
		if (_head_pose_handler.newData())
		{
			// Get latest data from tracker
			_head_pose_and_image = _head_pose_handler.getLastHeadPoseAndImage();
			// Upload image to texture
			_image_painter.setImage(_head_pose_and_image.second);
			// Reflect data around x-axis if showing mirrored image
			if (_mirror)
			{
				_head_pose_and_image.first.head_pos.x = -_head_pose_and_image.first.head_pos.x;
				_head_pose_and_image.first.head_rot.y_rads = -_head_pose_and_image.first.head_rot.y_rads;
				_head_pose_and_image.first.head_rot.z_rads = -_head_pose_and_image.first.head_rot.z_rads;
			}
			// Update face-to-world transform from new head-pose
			smARGetFaceToWorldTransformMatrix(_head_pose_and_image.first, &_face_to_world[0]);
		}

		// Draw texture on screen aligned quad
		_image_painter.draw(_mirror);

		// If tracking, render augmented reality 3D stuff over the top
		if (_head_pose_and_image.first.confidence > 0.0f)
		{
			// Set perspective viewing frustum
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(&_ar_projection[0]);

			// Set modelview matrix in order to set scene
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(&_ar_model_view[0]);

			double pitch = atan2(-(double)_face_to_world[6], (double)_face_to_world[10]);
			double yaw = atan2((double)_face_to_world[2], sqrt(1.0 - (double)(_face_to_world[2]*_face_to_world[2])));
			double roll= atan2(-(double)_face_to_world[1], (double)_face_to_world[0]);
			//
			edit_yaw->setText(QString::number(yaw*RADtoDEG, 'f',3));
			//setting background-color to white and text color as red for raw values for edit_yaw 
			edit_yaw->setStyleSheet("QLineEdit{background-color:white;color : red }");
			edit_pitch->setText(QString::number(pitch*RADtoDEG, 'f',3));
			//setting background-color to white and text color as red for raw values for edit_yaw 
			edit_pitch->setStyleSheet("QLineEdit{background-color:white;color : red }");
			edit_roll->setText(QString::number(roll*RADtoDEG, 'f',3));
			//setting background-color to white and text color as red for raw values for edit_yaw 
			edit_roll->setStyleSheet("QLineEdit{background-color:white;color : red }");
			
			double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
			if (_start)
			{
					if(counter < (calibration_timer->value()*30))
					{
					
						label_status_defined->setStyleSheet("QLineEdit{color : red }");
						label_status_defined->setText("Calibrating");
						yaw_ary[counter] = yaw;
						roll_ary[counter] = roll;
						pitch_ary[counter] = pitch;
						counter ++;					
					}
					if(counter == (calibration_timer->value()*30))
					{
						 
						cyaw = yaw-getMedian(yaw_ary, calibration_timer->value()*30);
						croll = roll-getMedian(roll_ary, calibration_timer->value()*30);
						cpitch = pitch-getMedian(pitch_ary, calibration_timer->value()*30);
						label_status_defined->setStyleSheet("QLineEdit{color : green }");
						label_status_defined->setText("Calibrated");
						counter++;
					}
					if(counter > (calibration_timer->value()*30))
					{
						cyaw = yaw-getMedian(yaw_ary, calibration_timer->value()*30);
						croll = roll-getMedian(roll_ary, calibration_timer->value()*30);
						cpitch = pitch-getMedian(pitch_ary, calibration_timer->value()*30);
						label_status_defined->setStyleSheet("QLineEdit{color : green }");
						label_status_defined->setText("Normal");
					}
			}

			if(calib_RButton->isChecked())
			{
			//plot_pitch->clearGraphs();
			plot_pitch->addGraph();
			plot_pitch->graph(1)->addData(key, cpitch*RADtoDEG);
			plot_pitch->graph(1)->setPen(QPen(Qt::green));
			plot_pitch->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
			plot_pitch->replot();
			//plot_yaw->clearGraphs();
			
			plot_yaw->addGraph();
			plot_yaw->graph(1)->addData(key, cyaw*RADtoDEG);
			plot_yaw->graph(1)->setPen(QPen(Qt::green));
			plot_yaw->xAxis->setRange(key+0.25, 8, Qt::AlignRight);	
			plot_yaw->replot();
			plot_roll->addGraph();
			plot_roll->graph(1)->addData(key, croll*RADtoDEG);
			plot_roll->graph(1)->setPen(QPen(Qt::green));
			plot_roll->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
			plot_roll->replot();
			}
			if(both_RButton->isChecked())
			{
				plot_pitch->addGraph();
				plot_pitch->graph(0)->addData(key, pitch*RADtoDEG);
				plot_pitch->graph(0)->setPen(QPen(Qt::red));
				plot_pitch->graph(1)->addData(key, cpitch*RADtoDEG);
				plot_pitch->graph(1)->setPen(QPen(Qt::blue));
				plot_pitch->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
				plot_pitch->replot();
				plot_yaw->addGraph();
				plot_yaw->graph(0)->addData(key, yaw*RADtoDEG);
				plot_yaw->graph(0)->setPen(QPen(Qt::red));
				plot_yaw->graph(1)->addData(key, cyaw*RADtoDEG);
				plot_yaw->graph(1)->setPen(QPen(Qt::blue));
				plot_yaw->xAxis->setRange(key+0.25, 8, Qt::AlignRight);	
				plot_yaw->replot();
				plot_roll->addGraph();
				plot_roll->graph(0)->addData(key, roll*RADtoDEG);
				plot_roll->graph(0)->setPen(QPen(Qt::red));
				plot_roll->graph(1)->addData(key, croll*RADtoDEG);
				plot_roll->graph(1)->setPen(QPen(Qt::blue));
				plot_roll->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
				plot_roll->replot();
			}
			
			if(raw_RButton->isChecked())
			{
			plot_pitch->addGraph();
			plot_pitch->graph(0)->addData(key, pitch*RADtoDEG);
			plot_pitch->graph(0)->setPen(QPen(Qt::red));
			plot_pitch->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
			plot_pitch->replot();
			plot_yaw->addGraph();
			plot_yaw->graph(0)->addData(key, yaw*RADtoDEG);
			//plot_yaw->graph(1)->addData(key, roll*RADtoDEG);
			plot_yaw->graph(0)->setPen(QPen(Qt::red));
			plot_yaw->xAxis->setRange(key+0.25, 8, Qt::AlignRight);	
			plot_yaw->replot();
			plot_roll->addGraph();
			plot_roll->graph(0)->addData(key, roll*RADtoDEG);
			//plot_roll->graph(1)->addData(key, pitch*RADtoDEG);
			plot_roll->graph(0)->setPen(QPen(Qt::red));
			plot_roll->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
			plot_roll->replot();
			}
			edit_cyaw->setText(QString::number(cyaw*RADtoDEG, 'f',3));
			edit_cyaw->setStyleSheet("QLineEdit{background-color:yellow;color : blue }");
			edit_cpitch->setText(QString::number(cpitch*RADtoDEG, 'f',3));
			edit_cpitch->setStyleSheet("QLineEdit{background-color:yellow;color : blue }");
			edit_croll->setText(QString::number(croll*RADtoDEG, 'f',3));
			edit_croll->setStyleSheet("QLineEdit{background-color:yellow;color : blue }");
			//time_start->setText(QDateTime::currentDateTimeUtc().toString());
	 

			
			// Draw axis at origin of face-coordinate system
			glMultMatrixf(&_face_to_world[0]);
			drawAxes();
		}
	}

void ARWindow::start()
{
	
	
	
		_start = true;
		_calibrate = false;
		yaw_ary[(calibration_timer->value()*30)];
		roll_ary[(calibration_timer->value()*30)];
		pitch_ary[(calibration_timer->value()*30)];
		cyaw = 0;
		cpitch = 0;
		croll = 0;
		counter = 0;
		//startButton->setEnabled(false);
		//endButton->setEnabled(true;
		time_start->setText(QDateTime::currentDateTime().toString());
		cout<<"hellow world";
		paintGL();
	
}

void ARWindow::stop()
{
	_start = false;
	_stop = true;
	time_stop->setText(QDateTime::currentDateTime().toString());
}
void ARWindow::plot()
{
	paintGL();
}
void ARWindow::threshold_control()
{
	if((threshold_top->text().toDouble()) < cyaw)// && (threshold_right->value() > cpitch) && (threshold_left->value() < cpitch))
{
	cout<<"hello world2";
	t_top->setText("looking up");
	t_top->setStyleSheet("QLineEdit{background-color:yellow;color : red }");
		
	//QPixmap pixmap = QPixmap("IMG_UP.png");
	//driver_image_label->setPixmap(pixmap);
}
}

/*
getMedian method this method is used for getting median value of collection of frames for the calibration time
this method takes double array containing frames for calibrated time. The size of this array is calibration time* 30 frames/seconds
this method returns median value in double format.
*/
double ARWindow::getMedian(double daArray[], int iSize)
{
	{
    // Allocate an array of the same size and sort it.
    double* dpSorted = new double[iSize];
	//copy elements of array to dpSorted array
    for (int i = 0; i < iSize; ++i) {
        dpSorted[i] = daArray[i];
    }
    for (int i = iSize - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (dpSorted[j] > dpSorted[j+1]) {
                double dTemp = dpSorted[j];
                dpSorted[j] = dpSorted[j+1];
                dpSorted[j+1] = dTemp;
            }
        }
    }

    // Middle or average of middle values in the sorted array.
    double dMedian = 0.0;
    if ((iSize % 2) == 0) {
        dMedian = (dpSorted[iSize/2] + dpSorted[(iSize/2) - 1])/2.0;
    } else {
        dMedian = dpSorted[iSize/2];
    }
    delete [] dpSorted;
	_calibrate = true;
	label_status_defined->setText("Calibrated");
    return dMedian;
}
}

void ARWindow::calibrate()
{
    start();
	
}
   

void ARWindow::drawAxes()
{
    const float cone_radius = 0.05f;
    const float cone_length = 0.1f;
    const float scale = 0.1f; // 10cm axes
    const float mirror_factor = _mirror ? -1.0f : 1.0f;
    const float alpha = 0.4f;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // X
	glColor4f(1.0f,0.0f,0.0f,alpha);
    glPushMatrix();
    glBegin(GL_LINES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(mirror_factor*scale,0.0f,0.0f);
    glEnd();
    glTranslatef(mirror_factor*scale,0.0f,0.0f);
	glRotatef(mirror_factor*90.0f,0.0f,1.0f,0.0f);
	glScalef(scale,0.0f,scale);
	glutWireCone(cone_radius,cone_length,4,1);
    glPopMatrix();	

    // Y
	glColor4f(0.0f,1.0f,0.0f,alpha);
    glPushMatrix();
    glBegin(GL_LINES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,scale,0.0f);
    glEnd();
    glTranslatef(0.0f,scale,0.0f);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	glScalef(scale,0.0f,scale);
	glutWireCone(cone_radius,cone_length,4,1);
    glPopMatrix();	

    // Z
	glColor4f(0.0f,0.0f,1.0f,alpha);
    glPushMatrix();
    glBegin(GL_LINES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,scale);
    glEnd();
    glTranslatef(0.0f,0.0f,scale);
	glScalef(scale,0.0f,scale);
	glutWireCone(cone_radius, cone_length,4,1);
    glPopMatrix();	
}
