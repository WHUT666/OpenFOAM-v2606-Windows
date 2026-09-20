/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2024-2025 OpenCFD Ltd.
    Copyright (C) 2026 Keysight Technologies
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
    Foam::functionObjects::cellDecomposer

Group
    grpFieldFunctionObjects

Description
    Maps input fields from local mesh to a secondary mesh at runtime.

    The secondary mesh gets created on-the-fly by decomposing the current mesh.

    The decomposition can be full (all cells) or partial (a cellSet). The
    decomposition can be done in different ways, see the \c decomposeType
    description below.

    The mapping is done via nearest-neighbour interpolation. The mapped fields
    are written to the time directory of the original mesh. The mapped fields
    have the same name as the input fields, but with a \c _decomposed suffix.

    The mapping tolerates cells that cannot be mapped, e.g. if the cell centre
    of a cell in the decomposed mesh lies outside the original mesh. The
    unmapped cells get a value of zero, and a warning is issued.

    Operands:
    \table
      Operand      | Type                       | Location
      input        | {vol,surface}<Type>Field   | <time>/inputFields
      output file  | -                          | -
      output field | {vol,surface}<Type>Field   | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    cellDecomposerFO
    {
        // Mandatory entries
        type            cellDecomposer;
        libs            (fieldFunctionObjects);
        fields          (<wordRes>);  // (<field1> <field2> ... <fieldN>);
        mapRegion       <word>;       // myTetMesh;
        decomposeType   <word>;       // polyhedral;
        selectionMode   <word>;       //all;

        faceSelectionMode <word>;   // see below, how to select faces

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd | Deflt
      type         | Type name: cellDecomposer          | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      fields       | Names of operand fields            | wordRes |  yes  | -
      cacheDecomposition | Cache the generated mesh    | bool |  no   | false
      mapRegion    | Name of region to map to           | word |  yes  | -
      decomposeType | How to decompose cells            | word |  yes  | -
      selectionMode | How to select cells (see fvOption)| word |  yes  | -
      faceSelectionMode | How to select faces           | word |  no   | -
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

    decomposeType:

    - faceCentre : decompose cells into tets using face centre and cell centre.
                   (hex becomes 6*4 tets)
    - faceDiagonal : decompose cells into tets using face diagonal, similar
                     to implicit decomposition inside lagrangian tracking.
                     (hex becomes 6*2 tets)
    - pyramid : keep faces intact but create (polygonal-base) pyramids using
                cell centre (hex becomes 6 pyramids)
    - faceDiagonalQuads : like faceDiagonal but split faces into quads and
                          triangles instead of just triangles
    - polyhedral : like faceDiagonalQuads but only decompose non-hex/prism/tet
                   cells in selected set. Used to convert polyhedral mesh into
                   'simple' mesh.

    Note: cacheDecomposition will
    - keep the mesh and maps
    - not update the points on the mesh
    It will not be correct if the selectionMode varies over time or if the
    geometry of the mapping is used (e.g. old face areas/volumes).

    The inherited entries are elaborated in:
     - \link functionObject.H \endlink

See also
    - Foam::functionObjects::mapFields

SourceFiles
    cellDecomposer.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_cellDecomposer_H
#define Foam_functionObjects_cellDecomposer_H

#include "fvMeshFunctionObject.H"
#include "volFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class tetDecomposer;
class mapPolyMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                       Class cellDecomposer Declaration
\*---------------------------------------------------------------------------*/

class cellDecomposer
:
    public fvMeshFunctionObject
{
    // Private Data

        //- Parameter dictionary
        dictionary dict_;

        //- Name of new mesh
        word mapRegion_;

        //- List of field names to interpolate
        wordRes fieldNames_;

        //- Whether to cache the decomposition
        bool cacheDecomposition_;

        //- Tet decomposer
        autoPtr<tetDecomposer> tetDecompPtr_;

        //- Map from polyMesh to tet-mesh
        autoPtr<mapPolyMesh> mapPtr_;


    // Private Member Functions

        //- Generate mesh
        void makeMesh(const dictionary& dict, const word& name);

        //- Helper function to map the \<Type\> fields
        template<class Type>
        bool mapFieldType() const;

        //- Helper function to write the \<Type\> fields
        template<class Type>
        bool writeFieldType() const;

        //- Interpolates a given volumetric field onto a new mesh using
        //- provided mapping information.
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh>>
        interpolate
        (
            const GeometricField<Type, fvPatchField, volMesh>& vf,
            const fvMesh& sMesh,
            const labelUList& patchMap,
            const labelUList& cellMap,
            const labelUList& faceMap,
            const bool allowUnmapped
        ) const;


public:


    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "cellDecomposer");


    // Constructors

        //- Construct from name, Time and dictionary
        cellDecomposer
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~cellDecomposer() = default;


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
    #include "cellDecomposerTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
