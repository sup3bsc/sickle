// Copyright 2015 PsychoLama

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWheelEvent>
#include <QKeyEvent>
#include <camera.hpp>
#include <cube.hpp>
#include <sphere.hpp>

/*! \brief Cadre de vue 3D
 *
 * Ce widget affiche une scène en 3D composée de géometries a partir d'une caméra.
 */
class Viewport : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
	explicit Viewport(QWidget* parent = nullptr);
	~Viewport();

	QSize minimumSizeHint() const Q_DECL_OVERRIDE {
		return QSize(500, 500);
	}

	QSize sizeHint() const Q_DECL_OVERRIDE {
		return QSize(500, 500);
	}

	template<typename T>
	T* addChild() {
		makeCurrent();
		auto child = new T(this);
		doneCurrent();
		emit childAdded(child);
		return child;
	}

	propRO(Camera*, camera);
	prop(GLenum, renderMode);
	propSig(bool, isInitialized, initialized);

#ifdef UNIT_TEST
	void updateNow() {
		makeCurrent();
		paintGL();
		doneCurrent();
	}
#endif

public slots:
	void save(QString name);
	void load(QString name);
	void clearLevel();

signals:
	void childAdded(QObject* child);

protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	int heightForWidth(int w) const { return w; }
	bool hasHeightForWidth() const { return true; }

private:
	QMatrix4x4 m_projection;
	QPoint m_cursor;
};

QDataStream& operator<<(QDataStream&, const QObject&);
QDataStream& operator>>(QDataStream&, QObject&);

#endif
