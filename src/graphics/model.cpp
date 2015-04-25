
#include <cmath>
#include <stdexcept>
#include "model.h"
#include "math/gamemath.h"

int getNextModelID()
{
    static int modelID = 0;
    return modelID++;
}

AABB Model::getAABB()
{
	return aabb;
}

AABB Model::generateAABB()
{
	if (data.size() == 0)
	{
		return AABB(0, 0, 0, 0, 0, 0);
	}
	float minX =  10000000;
	float minY =  10000000;
	float minZ =  10000000;
	float maxX = -10000000;
	float maxY = -10000000;
	float maxZ = -10000000;

	for (auto mesh : data)
	{
		auto cs = mesh->combinedData.size();
		for (int i = 0; i < cs; i += mesh->elementsPerRowOfCombinedData)
		{
			// X
			if (mesh->combinedData[i] > maxX)
			{
				maxX = mesh->combinedData[i];
			}
			if (mesh->combinedData[i] < minX)
			{
				minX = mesh->combinedData[i];
			}
			// Y
			if (mesh->combinedData[i] > maxY)
			{
				maxY = mesh->combinedData[i];
			}
			if (mesh->combinedData[i] < minY)
			{
				minY = mesh->combinedData[i];
			}
			// Z
			if (mesh->combinedData[i] > maxZ)
			{
				maxZ = mesh->combinedData[i];
			}
			if (mesh->combinedData[i] < minZ)
			{
				minZ = mesh->combinedData[i];
			}
		}
	}
	this->aabb = AABB(minX, minY, minZ, maxX, maxY, maxZ);
	return this->aabb;
}

Model::Model(std::vector<std::shared_ptr<MeshData>> data) : modelID(getNextModelID()), origin(glm::vec3(0, 0, 0)), rotationOnAxes(glm::vec3(0, 0, 0)),
        data(data), aabb(generateAABB()), scale(glm::vec3(1.0f, 1.0f, 1.0f))
{
}

Model::~Model()
{
}

bool Model::intersections(AABB &other)
{
	return intersects(aabb, other);
}

glm::vec3 Model::getOrigin()
{
    return origin;
}

void Model::setOrigin(glm::vec3 origin)
{
    this->origin = origin;
}

void Model::scaleModel(glm::vec3 scale)
{
    this->scale = scale;
}

void Model::scaleModel(float amount, bool scaleX, bool scaleY, bool scaleZ)
{
    if (scaleX)
        scale = glm::vec3(scale.x * amount, scale.y, scale.z);
    if (scaleY)
        scale = glm::vec3(scale.x, scale.y * amount, scale.z);
    if (scaleZ)
        scale = glm::vec3(scale.x, scale.y, scale.z * amount);
}

void Model::translate(glm::vec3 amount)
{
    origin = origin + amount;
}

glm::vec3 Model::getRotationOnAxes()
{
    return rotationOnAxes;
}

void Model::rotate(glm::vec3 rotation)
{
    setRotationOnAxes(glm::vec3(rotation.x + rotationOnAxes.x,
                                rotation.y + rotationOnAxes.y,
                                rotation.z + rotationOnAxes.z));
}

void Model::reduceRotation()
{
    setRotationOnAxes(glm::vec3(fmod(rotationOnAxes.x, (2 * PI)),
                                fmod(rotationOnAxes.y, (2 * PI)),
                                fmod(rotationOnAxes.z, (2 * PI))));
}

void Model::setRotationOnAxes(glm::vec3 rotationOnAxes)
{
    this->rotationOnAxes = rotationOnAxes;
}

int Model::getID()
{
    return modelID;
}

void Model::createVBOs(std::map<std::string, std::shared_ptr<Texture>> textureMap)
{
    for(unsigned int i = 0; i < data.size(); i++)
    {
        std::cout << ">" << data[i]->associatedTextureName << "<"<< std::endl;
        if(overrideTexture)
        {
            vbos.push_back(std::shared_ptr<VBO>(new VBO(data[i], overrideTexture)));
        }
        else
        {
            vbos.push_back(std::shared_ptr<VBO>(new VBO(data[i], textureMap[data[i]->associatedTextureName])));
        }
    }
}

void Model::draw(Camera *camera)
{
    for(unsigned int i = 0; i < vbos.size(); i++)
    {
        vbos[i]->draw(camera);
    }
}



