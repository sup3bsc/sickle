// Copyright 2015 PsychoLama

#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <globals.hpp>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QMetaProperty>

class Viewport;

enum RenderBuffer {
	RB_DEPTH,
	RB_SCENE
};

/*! \brief Structure contenant les infos sur la frame courante
 *
 * Cette structure contient toutes les informations utiles pour les acteurs a propos de l'image en cours de rendu:
 * les matrices de vue et de projection, le mode de rendu (plein ou fil de fer), ainsi que le contexte de rendu.
 * Cette structure est passée par référence a tous les acteurs lors du rendu. Cela limite la consommation de mémoire,
 * et évite les longues listes d'arguments (dont tous ne sont pas toujours utilisés). Cela permet aussi d'ajouter de
 * nouvelles informations sans impacter les classes existantes.
 */
typedef struct DrawInfo {
	GLenum mode;
	QOpenGLContext* context;
	RenderBuffer buffer;
	QVariantHash uniforms;
} DrawInfo;

/*! \brief Base de tous les objets de la scène
 *
 * Cette classe est la classe de base de tous les acteurs, dont hérite toutes les classes pouvant être ajoutées a un
 * monde. Elle définit les propriétés de bases d'un acteur, un constructeur de copie utilisant les méta-classes de Qt,
 * ainsi que les méthodes virtuelles a implémenter par les classes enfants.
 */
class Actor : public QObject {
	Q_OBJECT

	public:
		explicit Actor(QObject* parent = nullptr);
		Actor(const Actor& copy) : QObject(copy.parent()) {
			auto metaObject = copy.metaObject();
			for (int i = 0; i < metaObject->propertyCount(); ++i) {
				auto prop = metaObject->property(i);
				setProperty(prop.name(), copy.property(prop.name()));
			}
		}
		virtual void draw(const DrawInfo& info) = 0;
		virtual void setParent(QObject* parent);
		virtual bool event(QEvent* event);

		propSig(QVector3D, position, moved);
		propSig(QQuaternion, orientation, rotated);
		propSig(QVector3D, scale, scaled);

	protected:
		virtual QMatrix4x4 transform();

		template<typename T>
		T* findParent() {
			QObject* par = this;
			do {
				par = par->parent();
			} while (par != nullptr && dynamic_cast<T*>(par) == nullptr);
			return dynamic_cast<T*>(par);
		}

		Viewport* viewport();

private:
		Viewport* m_viewport;
};

Q_DECLARE_INTERFACE(Actor, "com.sup3asc2.sickle.Actor/1.0")

#endif  // ACTOR_HPP

