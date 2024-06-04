#include "A3GLWidget.h"

#include <iostream>

A3GLWidget::A3GLWidget (QWidget* parent) : MyGLWidget(parent)
{
	torxesPos[0] = glm::vec3(-7.39, 1.95, -6.68);
    torxesPos[1] = glm::vec3(-9.95, 1.95, -0.56);
    torxesPos[2] = glm::vec3(-7.47, 1.95, 5.64);
    torxesPos[3] = glm::vec3(4.38, 1.95, 5.26);
    torxesPos[4] = glm::vec3(6.68, 1.95, 0.38);
    torxesPos[5] = glm::vec3(4.15, 1.95, -6.97);

	for(unsigned int i = 0; i < 6; i++)
	{
		torxesCol[i] = glm::vec3(1.0,1.0,0.0);
	}
}

A3GLWidget::~A3GLWidget()
{
}


void A3GLWidget::iniEscena ()
{
	creaBuffersModels();

	VaixellPos = glm::vec3(-10, 0.0, 0.0);
	//--------------------------------------------------
	centreEsc = glm::vec3 (0, 0, 0);
	radiEsc = 20;// Tret de la màniga :-P
}


void A3GLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

	// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
	// useu els paràmetres que considereu (els que hi ha són els de per defecte)
	//  glViewport (0, 0, ample, alt);
	estableixFocus();
	projectTransform ();
	viewTransform ();
	// Esborrem el frame-buffer i el depth-buffer

	glClearColor(0.8f, 0.8f, 1.0f, 1.f);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//--------------------------------------------------------
	// Activem el VAO per a pintar el vaixell
	glBindVertexArray (VAO_models[VAIXELL]);
	// Transformació geometrica
	modelTransformVaixell();
	posaTorxes();
	// pintem el vaixell
	glDrawArrays(GL_TRIANGLES, 0, models[VAIXELL].faces().size()*3);
	//--------------------------------------------------------
	// Activem el VAO per a pintar el moll
	glBindVertexArray (VAO_models[MOLL]);
	// pintem les espelmes, cadascuna amb el seu transform
	_normalMatrix = glm::inverseTranspose(glm::mat3(View*modelTransformMoll()));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &_normalMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, models[MOLL].faces().size()*3);
	//--------------------------------------------------------
	// Activem el VAO per a pintar el far (primera part)
	glBindVertexArray (VAO_models[FAR_1]);
	// pintem les espelmes, cadascuna amb el seu transform
	modelTransformFar1();
	glDrawArrays(GL_TRIANGLES, 0, models[FAR_1].faces().size()*3);
	//--------------------------------------------------------
	// Activem el VAO per a pintar el far (segona part)
	glBindVertexArray (VAO_models[FAR_2]);
	// pintem les espelmes, cadascuna amb el seu transform
	modelTransformFar2();
	faro1();
	faro2();
	glDrawArrays(GL_TRIANGLES, 0, models[FAR_2].faces().size()*3);
	//------------------------------------------------------------
	//Pintem terra
	glBindVertexArray(VAO_Terra);
	_normalMatrix = glm::inverseTranspose(glm::mat3(View*modelTransformTerra()));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &_normalMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
	// Pintem Mar
	glBindVertexArray(VAO_Mar);
	_normalMatrix = glm::inverseTranspose(glm::mat3(View*modelTransformMar()));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &_normalMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
	//--------------------------------------------------------
	glBindVertexArray(0);
}

void A3GLWidget::modelTransformFar1()
{
	// Codi per a la TG necessària
	glm::mat4 TG = glm::mat4(1.0f);
	TG = glm::translate(TG, glm::vec3(2, 1, 4));
	TG = glm::scale(TG, glm::vec3(escalaModels[FAR_1]) );
	TG = glm::translate(TG, -centreBaseModels[FAR_1]);
	_normalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &_normalMatrix[0][0]);
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void A3GLWidget::modelTransformFar2()
{
	// Codi per a la TG necessària
	Far2_TG = glm::mat4(1.0f);
	Far2_TG = glm::translate(Far2_TG, glm::vec3(2, 1, 4));
	Far2_TG = glm::rotate(Far2_TG, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	Far2_TG = glm::scale(Far2_TG, glm::vec3(escalaModels[FAR_1]) );
	Far2_TG = glm::translate(Far2_TG, -centreBaseModels[FAR_1]);
	_normalMatrix = glm::inverseTranspose(glm::mat3(View*Far2_TG));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &_normalMatrix[0][0]);
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &Far2_TG[0][0]);
}


void A3GLWidget::modelTransformVaixell()
{
	// Codi per a la TG necessària
	Vaixell_TG = glm::mat4(1.0f);
	Vaixell_TG = glm::translate(Vaixell_TG, VaixellPos);
	Vaixell_TG = glm::scale(Vaixell_TG, glm::vec3(escalaModels[VAIXELL]) );
	Vaixell_TG = glm::translate(Vaixell_TG,	-glm::vec3(centreBaseModels[VAIXELL].x,0,centreBaseModels[VAIXELL].z)); // el vaixell ja esta a la línia de flotació....no toquem en alçada
	_normalMatrix = glm::inverseTranspose(glm::mat3(View*Vaixell_TG));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &_normalMatrix[0][0]);
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &Vaixell_TG[0][0]);
}

void A3GLWidget::sunMovement(int hour)
{
	float t = (hour - 8.0f) / 12.0f; // Normaliza el tiempo a [0, 1]
    float angle = t * M_PI; // Mapea el tiempo a [0, PI] para la semicircunferencia

    sunPosition.x = -40.0f * cos(angle);
    sunPosition.y = 40.0f * sin(angle);
    sunPosition.z = 0.0f;
}


void A3GLWidget::keyPressEvent(QKeyEvent* event)
{
	makeCurrent();
    
    switch (event->key()) {
        case Qt::Key_S: {
            VaixellPos[2] += 1;
            break;
        }
        case Qt::Key_W: {
            VaixellPos[2] -= 1;
            break;
        }
        case Qt::Key_Up: {
            if (actualHour < 20.0f) actualHour += 1.0f;
            sunMovement(actualHour);
		}
            break;
        case Qt::Key_Down: {
            if (actualHour > 8.0f) actualHour -= 1.0f;
            sunMovement(actualHour);
		}
            break;
		case Qt::Key_A: {
			angle += glm::radians(5.0f);
		}
			break;
		case Qt::Key_D: {
			angle -= glm::radians(5.0f);
		}
			break;
        default:
            event->ignore();
            return;
    }
    update();
}

void A3GLWidget::estableixFocus()
{
	glUniform3fv(focusPosLoc, 1, &sunPosition[0]);
	glm::vec3 col = glm::vec3(0.6,0.6,0.6);
	glUniform3fv(llumFocusLoc, 1, &col[0]);
	glm::vec3 llum = glm::vec3(0.1,0.1,0.1);
	glUniform3fv(llumAmbientLoc, 1, &llum[0]);
}

void A3GLWidget::faro1()
{
	glm::vec3 aux = glm::vec3(View*Far2_TG*glm::vec4(far1Position, 1.0));
	  // Multiplicar por la matriz
	//far1Position = glm::vec3(far1Position4);  // Convertir de nuevo a vec3
	glUniform3fv(faro1PosLoc, 1, &aux[0]);
	glUniform3fv(faro1ColLoc, 1, &far1Col[0]);
}

void A3GLWidget::faro2()
{
	//far2Position = glm::vec3(far2Position4);
	glm::vec3 aux = glm::vec3(View*Far2_TG*glm::vec4(far2Position, 1.0));
	glUniform3fv(faro2PosLoc, 1, &aux[0]);
	glUniform3fv(faro2ColLoc, 1, &far2Col[0]);
}

void A3GLWidget::posaTorxes()
{
	glm::vec3 aux[6];
	for(unsigned int i = 0; i < NUM_TORXES; i++)
    {
		aux[i] = glm::vec3(View*Vaixell_TG*glm::vec4(torxesPos[i], 1.0)); 
    }
	glUniform3fv(torxesPosLoc, NUM_TORXES, glm::value_ptr(aux[0]));
    glUniform3fv(torxesColLoc, NUM_TORXES, glm::value_ptr(torxesCol[0]));
}

void A3GLWidget::setHour(QTime aHour) {
    int hour = aHour.hour();  // Extrae la hora del objeto QTime
    if (hour >= 8 && hour <= 20) {
        actualHour = static_cast<float>(hour);  // Convierte la hora a float
        sunMovement(actualHour);  // Llama a la función que ajusta el movimiento del sol
        update();  // Fuerza el redibujado de OpenGL
    }
}


void A3GLWidget::carregaShaders()
{
	// Creem els shaders per al fragment shader i el vertex shader
	QOpenGLShader fs (QOpenGLShader::Fragment, this);
	QOpenGLShader vs (QOpenGLShader::Vertex, this);
	// Carreguem el codi dels fitxers i els compilem
	fs.compileSourceFile("./shaders/basicLlumShader.frag");
	vs.compileSourceFile("./shaders/basicLlumShader.vert");
	// Creem el program
	program = new QOpenGLShaderProgram(this);
	// Li afegim els shaders corresponents
	program->addShader(&fs);
	program->addShader(&vs);
	// Linkem el program
	program->link();
	// Indiquem que aquest és el program que volem usar
	program->bind();

	// Obtenim identificador per a l'atribut “vertex” del vertex shader
	vertexLoc = glGetAttribLocation (program->programId(), "vertex");
	// Obtenim identificador per a l'atribut “normal” del vertex shader
	normalLoc = glGetAttribLocation (program->programId(), "normal");
	// Obtenim identificador per a l'atribut “matamb” del vertex shader
	matambLoc = glGetAttribLocation (program->programId(), "matamb");
	// Obtenim identificador per a l'atribut “matdiff” del vertex shader
	matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
	// Obtenim identificador per a l'atribut “matspec” del vertex shader
	matspecLoc = glGetAttribLocation (program->programId(), "matspec");
	// Obtenim identificador per a l'atribut “matshin” del vertex shader
	matshinLoc = glGetAttribLocation (program->programId(), "matshin");

	// Demanem identificadors per als uniforms del vertex shader
	transLoc = glGetUniformLocation (program->programId(), "TG");
	projLoc = glGetUniformLocation (program->programId(), "proj");
	viewLoc = glGetUniformLocation (program->programId(), "view");
	normalMatrixLoc = glGetUniformLocation(program->programId(), "normalMatrix");

	// Bloc d'uniforms
	llumFocusLoc = glGetUniformLocation(program->programId(), "llumFocus");
	focusPosLoc = glGetUniformLocation(program->programId(), "posFocus");
	llumAmbientLoc = glGetUniformLocation(program->programId(), "llumAmbient");

	faro1PosLoc = glGetUniformLocation(program->programId(), "faro1Pos");
	faro1ColLoc = glGetUniformLocation(program->programId(), "faro1Col");

	faro2ColLoc = glGetUniformLocation(program->programId(), "faro2Col");
	faro2PosLoc = glGetUniformLocation(program->programId(), "faro2Pos");

	torxesPosLoc = glGetUniformLocation(program->programId(), "torxesPos");
	torxesColLoc = glGetUniformLocation(program->programId(), "torxesCol");
}
