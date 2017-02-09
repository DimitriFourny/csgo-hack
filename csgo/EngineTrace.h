#pragma once

#include "GameInterface.h"
#include "Vector4.h"
#include "GameTrace.h"
#include "vmtIndexes.h"


struct Matrix3x4 {
	float m_matVal[3][4];
};

struct Ray {
	Ray() { }

	Vector4	m_start;
	Vector4 m_delta;
	Vector4 m_startOffset;
	Vector4 m_extents;
	const Matrix3x4* m_worldAxisTransform;
	bool m_isRay;
	bool m_isSwept;

	void init(Vector3 vecStart, Vector3 vecEnd) {
		m_delta = vecEnd - vecStart;
		m_isSwept = (m_delta.lengthSquare() != 0);
		m_extents.reset();
		m_worldAxisTransform = NULL;
		m_isRay = true;
		m_startOffset.reset();
		m_start = vecStart;
	}
};

enum TraceType {
	TRACE_EVERYTHING,
	TRACE_WORLD_ONLY,				
	TRACE_ENTITIES_ONLY,			
	TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(BaseEntity* pEntityHandle, int contentsMask) = 0;
	virtual TraceType GetTraceType() = 0;
};
class TraceFilter : ITraceFilter {
public:
	virtual bool ShouldHitEntity(BaseEntity* pEntityHandle, int contentsMask) {
		return pEntityHandle != pSkip;
	}
	virtual TraceType GetTraceType() {
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};


class EngineTrace : public GameInterface {
public:
	using GameInterface::GameInterface;

	void TraceRay(const Ray& ray, unsigned int mask, TraceFilter* pTraceFilter, GameTrace* pTrace);
};