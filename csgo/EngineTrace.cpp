#include "EngineTrace.h"

void EngineTrace::TraceRay(const Ray& ray, unsigned int mask, TraceFilter* pTraceFilter, GameTrace* pTrace) {
	typedef void(__thiscall *TraceRayFn)(PVOID pThis, const Ray& ray, unsigned int mask, TraceFilter* pTraceFilter, GameTrace* pTrace);
	TraceRayFn oTraceRay = (TraceRayFn)m_vtable->GetMethodAddress(VMT_TRACERAY);
	oTraceRay(self(), ray, mask, pTraceFilter, pTrace);
}