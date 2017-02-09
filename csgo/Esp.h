#pragma once

#include "BaseEntity.h"

class Esp {
public:
	Esp(Loader* loader);
	void drawSkeleton(BaseEntity& entity);
	void drawBox(BaseEntity& entity);

private:
	void setEspColor(BaseEntity& entity);

	Loader* m_loader;
};