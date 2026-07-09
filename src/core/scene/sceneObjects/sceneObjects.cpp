#include "sceneObjects.h"

void sceneObjects::addModel(instancedModel& iModel)
{
	m_sceneModels.push_back(iModel);
}

void sceneObjects::addLight(instancedLight& iLight)
{
	m_sceneLights.push_back(iLight);
}

