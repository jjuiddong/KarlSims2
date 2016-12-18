// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>




///////////////////////////////////////////////////////////////////////////////
// PhysX Sample Renderer
#include <Renderer.h>
#include <RendererVertexBuffer.h>
#include <RendererVertexBufferDesc.h>
#include <RendererIndexBuffer.h>
#include <RendererIndexBufferDesc.h>
#include <RendererMesh.h>
#include <RendererMeshDesc.h>
#include <RendererMemoryMacros.h>
#include <RendererShape.h>
#include <RaycastCCD.h>
#include <RenderMaterial.h>
#include <RenderBaseActor.h>
#include <d3d9.h>



///////////////////////////////////////////////////////////////////////////////
// PhysX
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"

using namespace physx::shdfnd;
using namespace physx;



///////////////////////////////////////////////////////////////////////////////
#include "../../../../Common/Common/Common.h"
using namespace common;

//#include "../wxMemMonitorLib/wxMemMonitor.h"
#include <boost/algorithm/string.hpp>
#include <set>
#include <mmsystem.h>

using std::list;
using std::vector;
using std::map;
using std::set;
using std::string;
using std::stringstream;

#ifndef u_int
typedef unsigned int u_int;
#endif


//
/////////////////////////////////////////////////////////////////////////////////
//// Global Variabl
//#include "Configure.h"
//extern SDbgConfig *g_pDbgConfig;
//
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Global Function
//#include "RenderBoxActor.h"
//#include "RenderSphereActor.h"
//#include "Picking.h"
//#include "SampleBaseInputEventIds.h"
//
//#include "utility/Utility.h"
//#include "utility/FileLoader.h"
//#include "renderer/RenderModelActor.h"
//#include "renderer/RenderBezierActor.h"
//#include "renderer/RenderComposition.h"
//#include "renderer/RendererCompositionShape.h"
//
//#include "global.h"


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx9d.lib")

