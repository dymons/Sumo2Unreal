#pragma once
#include "ContainersAndTypes.h"
#include "Editor.h"
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "LightingBuildOptions.h"
#include "Runtime/Core/Public/Containers/Map.h"
#include "Runtime/XmlParser/Public/FastXml.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "StopSignMesh.h"

#include <memory>
#include <stack>

// forward declarations

class UfileParser final : public IFastXmlCallback
{
    template<typename Type>
    void DestroyActorsByType() const
    {
        TArray<AActor*> Actors;
        UGameplayStatics::GetAllActorsOfClass(World, Type::StaticClass(), Actors);
        for (AActor*& Actor : Actors)
        {
            World->DestroyActor(Actor);
        }
    }

public:
    UfileParser(const TCHAR*);

    // IFastXMLCallback
    bool ProcessXmlDeclaration(const TCHAR* ElementData, int32 XmlFileLineNumber);
    bool ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber);
    bool ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue);
    bool ProcessClose(const TCHAR* Element);
    bool ProcessComment(const TCHAR* Comment);
    bool loadxml();

    // Spline Container
    splineCont SplineContainer;
    FString turnType = "";

    // Put the shape coordinates into Shapecoordinates vector
    void ShapeProcessing(const TCHAR*);

    //----Functions for initializing the node objects----

    // Collect all required node attributes for one object as member variables and then initialize a node object
    void InitializeNodeAttributes(const TCHAR*, const TCHAR*);
    SimpleNodePtr InitializeNode();
    TArray<FString> incomingLaneContainer;

    // one container object to store all node mappings
    NodeCont NodeContainer;

    // Getter and setter methods of temporary node ID for the latest node created
    FString getTempNodeID();
    bool setTempNodeID(const TCHAR*);

    //----Functions for initializing the walkingArea objects----

    // Collect all required node attributes for one object as member variables and then initialize a node object
    void InitializeWalkingAreaAttributes(const TCHAR*, const TCHAR*);
    void InitializewalkingArea();

    // one container object to store all node mappings
    walkingAreaCont walkingAreaContainer;

    //----Functions for initializing the edge objects----
    // Collect all the required internal edge attributes (specifically for splines which are within a junction)
    void InitializeInternalEdgeAttributes(const TCHAR*, const TCHAR*);
    void MakeSpline(std::vector<float>&, FVector&);
    void MakeSpline();

    // Collect all the required edge attributes for one object as member variables and then initialize a edge object
    void InitializeEdgeAttributes(const TCHAR*, const TCHAR*);
    SimpleEdgePtr InitializeEdge(const TCHAR*);
    SimpleEdgePtr InitializePedestrianEdge();

    // Container object for edge mappings
    EdgeCont EdgeContainer;

    // To calculate Lane width
    // void calculateLaneWidth();
    float laneWidth;

    //----------------Functions for initialzing Traffic Lights and Stop signs----------------
    void InitializetrafficLightAttributes(
        const TCHAR*, const TCHAR*);                // Traffic light id is currently being used to place stop signs also.
    void InitializeTrafficControl(const TCHAR*);    // Used for either initializing traffic lights and stop signs.
    void LinkTrafficControlToSplines();             // Links all incoming splines to a junction to the closest traffic lights
    TArray<FVector> finalSplinePoints;

    StopSignCont StopSignContainer;

    //---------------Functions for initializing spline attributes--------------------
    void InitializeSplineAttributes(const TCHAR*, const TCHAR*);

private:
    UWorld* World = GEditor->GetEditorWorldContext().World();
    std::vector<float> Shapecoordinates;
    FString selectedXMLFile;
    TArray<AActor*> FoundActors;
    FLightingBuildOptions LightOptions;

    // node related flags and member variables
    bool isElementNode = false;    // flag
    bool isPriorityNode = false;
    bool isTrafficNode = false;
    const TCHAR* nodeXCoordinate;    // attribute
    const TCHAR* nodeYCoordinate;
    bool xCoordinateIsSet = false;
    bool yCoordinateIsSet = false;
    bool laneWidthIsSet = false;
    TArray<TUniquePtr<FString>> trafficControlIDList;    // List of node IDs which are of traffic light or stop sign type.

    // edge flags and temp member variables
    bool isInternalEdge = false;
    bool isElementEdge = false;
    FString fromNode = "";
    FString toNode = "";
    FString laneLength = "";
    bool fromNodeSet = false;
    bool toNodeSet = false;
    bool lengthIsSet = false;

    void resetFlagsAndTempMembers();
    bool shapeIsSet = false;

    // lane parameters
    bool isElementLane = false;

    // walking area member variables
    bool isWalkingArea = false;
    bool doesWalkingAreaExist = false;    // useful when trying to spawn the traffic light.

    // pedestrian crossing member variables
    bool isCrossing = false;

    // sidewalk member variables
    bool isSidewalk = false;

    // traffic light member variables
    bool isElementtrafficLight = false;

    // connection member variables
    bool isConnection = false;
    FString fromID = "";
    FString toID = "";
    FString viaID = "";
    bool viaIsSet;

    // temporary ID
    FString tempNodeID = "";
    FString tempEdgeID = "";
    FString walkingAreaID = "";
    FString walkingAreaKeyID = "";
    FString tempTrafficLightID = "";
};
