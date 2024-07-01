#pragma once
class PlaneMeshData {
	float width;
	float height;
};
class PlanMesh {
public:
	void initialize(const PlaneMeshData& data);

private:
	PlaneMeshData data;

};

