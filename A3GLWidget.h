#ifndef A3GLWIDGET_H
#define A3GLWIDGET_H

#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTime>  // Incluir para usar QTime en el slot
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "model.h"
#include "MyGLWidget.h"

class A3GLWidget : public MyGLWidget {
    Q_OBJECT

public:
    explicit A3GLWidget(QWidget *parent = nullptr);
    ~A3GLWidget();
signals:
    void hourChanged(QTime newHour); 

public slots:
    void setHour(QTime aHour);

protected:
    void modelTransformFar1();
    void modelTransformFar2();
    void modelTransformVaixell();
    void iniEscena();
    void carregaShaders();
    void estableixFocus();
    void faro1();
    void faro2();
    void posaTorxes();
    void sunMovement(int n);

    virtual void paintGL();
    virtual void keyPressEvent(QKeyEvent *event);

    // uniform locations
    GLuint llumFocusLoc, focusPosLoc, llumAmbientLoc;
    GLuint normalMatrixLoc;
    GLuint faro1PosLoc, faro2PosLoc;
    GLuint faro1ColLoc, faro2ColLoc;
    GLuint torxesPosLoc, torxesColLoc;


    // Posicions
    glm::vec3 VaixellPos;
    glm::vec3 sunPosition = glm::vec3(0, 40, 0);

    glm::vec3 far1Position = glm::vec3(0.363,4.695,0.357);
    glm::vec3 far1Col = glm::vec3(1);

    glm::vec3 far2Position = glm::vec3(-0.357,4.695,-0.348);
    glm::vec3 far2Col = glm::vec3(1);

    glm::vec3 torxesPos[6];
    glm::vec3 torxesCol[6];


    // MATRIUS DE TRANSFORMACIÓ
    glm::mat4 Vaixell_TG, Far2_TG;
    glm::mat3 _normalMatrix;

    // Variables to manage sunlight based on time
    int sunRotation = 0;
    int actualHour = 14;

    float angle;

    static constexpr int NUM_TORXES = 6;
    static constexpr int HORA_MIN = 8;
    static constexpr int HORA_MAX = 20;
    static constexpr int NUM_FOCUS_FAR = 2;
};

#endif // A3GLWIDGET_H
