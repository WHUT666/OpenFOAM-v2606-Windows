/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2025 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::functionObjects::mapFields

Group
    grpFieldFunctionObjects

Description
    Maps input fields from local mesh to secondary mesh at runtime.

    Operands:
    \table
      Operand      | Type                       | Location
      input        | {vol,surface}<Type>Field   | <time>/inputField
      output file  | -                          | -
      output field | {vol,surface}\<Type\>Field | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    mapFieldsFO
    {
        // Mandatory entries
        type            mapFields;
        libs            (fieldFunctionObjects);
        fields          <wordList>;  // (<field1> <field2> ... <fieldN>);
        mapRegion       <word>;      // coarseMesh;
        mapMethod       <word>;      // cellVolumeWeight;
        consistent      <bool>;;

        // Optional entries
        patchMapMethod  direct;  // AMI-related entry

        // Conditional entries

        // when consistent=false

            // Manual: createPatchMap false;
            // patchMap        (<patchSrc> <patchTgt>);
            // cuttingPatches  (<patchTgt1> <patchTgt2> ... <patchTgtN>);

            // Automatic: createPatchMap true;
            // Creates the patchMap and cuttingPatches automatically

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: mapFields               | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      fields       | Names of operand fields            | wordList |  yes  | -
      mapRegion    | Name of region to map to           | word |  yes  | -
      mapMethod    | Mapping method                     | word |  yes  | -
      consistent   | Mapping meshes have consistent boundaries | bool  | yes | -
      patchMapMethod | Patch mapping method for AMI cases | word |  no  | -
      createPatchMap | Automatically create patchMap and cuttingPatches|  <!--
                 --> | bool | no | -
      patchMap     | Mapping between coincident source and target patches <!--
               --> | wordHashTable | no | -
      cuttingPatches | Target patches cutting the source domain <!--
                 --> | wordList | no | -
    \endtable

    Options for the \c mapMethod entry:
    \verbatim
      direct
      mapNearest
      cellVolumeWeight
      correctedCellVolumeWeight
    \endverbatim

    Options for the \c patchMapMethod entry:
    \verbatim
      nearestFaceAMI
      faceAreaWeightAMI
    \endverbatim

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    mapFields.C
    mapFieldsTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_mapFields_H
#define Foam_functionObjects_mapFields_H

#include "fvMeshFunctionObject.H"
#include "volFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class meshToMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class mapFields Declaration
\*---------------------------------------------------------------------------*/

class mapFields
:
    public fvMeshFunctionObject
{
    // Private Data

        //- Input dictionary
        dictionary dict_;

        // //- Locally cached map region mesh (map to this mesh)
        // autoPtr<fvMesh> mapRegionPtr_;

        //- Mesh-to-mesh interpolation
        autoPtr<meshToMesh> interpPtr_;

        //- List of field names to interpolate
        wordRes fieldNames_;


    // Private Member Functions

        //- On-the-fly lookup or load of the map region mesh
        const fvMesh& lookupMapRegion() const;

        //- Helper function to create the mesh-to-mesh interpolation
        void createInterpolation
        (
            const dictionary& dict,
            const fvMesh& meshTarget,
            const fvMesh& mapRegion
        );

        // //- Helper function to create the mesh-to-mesh interpolation
        // void createInterpolation(const dictionary& dict);

        //- Helper function to evaluate constraint patches after mapping
        template<class Type>
        void evaluateConstraintTypes
        (
            GeometricField<Type, fvPatchField, volMesh>& fld
        ) const;

        //- Helper function to map the \<Type\> fields
        template<class Type>
        bool mapFieldType() const;

        //- Helper function to write the \<Type\> fields
        template<class Type>
        bool writeFieldType() const;


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "mapFields");


    // Constructors

        //- Construct from name, Time and dictionary
        mapFields
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        mapFields(const mapFields&) = delete;

        //- No copy assignment
        void operator=(const mapFields&) = delete;


    //- Destructor
    virtual ~mapFields() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);

        //- Execute the function-object operations
        virtual bool execute();

        //- Write the function-object results
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "mapFieldsTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
