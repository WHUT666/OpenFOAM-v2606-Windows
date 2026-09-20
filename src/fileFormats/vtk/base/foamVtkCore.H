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

Namespace
    Foam::vtk

Description
    Namespace for handling VTK output.
    Contains classes and functions for writing VTK file content.

Namespace
    Foam::vtk::legacy

Description
    Namespace for legacy VTK output constants and functions.

Namespace
    Foam::vtk::Tools

Description
    A collection of static methods to help with conversion to/from VTK data.

SourceFiles
    foamVtkCore.cxx
    foamVtkCore.txx

\*---------------------------------------------------------------------------*/

#ifndef Foam_vtk_core_H
#define Foam_vtk_core_H

#include <cstdint>

#include "Enum.H"
#include "pTraits.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
template<class Cmpt> class SymmTensor;

namespace vtk
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    // Enumerations

    //- The context when outputting a VTK file (XML or legacy).
    enum OutputContext : uint8_t
    {
        INLINE,     //!< Generate header and inline data
        HEADER,     //!< Generate header only
        APPEND      //!< Generate append-data
    };


    //- The output format type for file contents.
    //  Upper bits for output type, lower bits for the format itself.
    enum class formatType : uint8_t
    {
        INLINE_ASCII  = 0,    //!< XML inline ASCII, asciiFormatter
        INLINE_BASE64 = 0x01, //!< XML inline base64, base64Formatter
        APPEND_BASE64 = 0x11, //!< XML append base64, appendBase64Formatter
        APPEND_BINARY = 0x12, //!< XML append raw binary, appendRawFormatter
        LEGACY_ASCII  = 0x20, //!< Legacy ASCII, legacyAsciiFormatter
        LEGACY_BINARY = 0x22, //!< Legacy raw binary, legacyRawFormatter
        HDF_FORMAT = 0x40,    //!< VTKHDF format
    };

    //- Test for vtk append format (xml)
    inline bool isAppend(enum formatType fmt) noexcept
    {
        return (uint8_t(fmt) & 0x10);
    }

    //- Test for vtk legacy format
    inline bool isLegacy(enum formatType fmt) noexcept
    {
        return (uint8_t(fmt) & 0x20);
    }

    //- Test for HDF format
    inline bool isHDF(enum formatType fmt) noexcept
    {
        return (formatType::HDF_FORMAT == fmt);
    }


    //- Equivalent to enumeration in "vtkCellType.h" (should be uint8_t)
    enum cellType : uint8_t
    {
        VTK_EMPTY_CELL       = 0,
        VTK_VERTEX           = 1,
        VTK_POLY_VERTEX      = 2,
        VTK_LINE             = 3,
        VTK_POLY_LINE        = 4,
        VTK_TRIANGLE         = 5,
        VTK_TRIANGLE_STRIP   = 6,
        VTK_POLYGON          = 7,
        VTK_PIXEL            = 8,
        VTK_QUAD             = 9,
        VTK_TETRA            = 10,
        VTK_VOXEL            = 11,
        VTK_HEXAHEDRON       = 12,
        VTK_WEDGE            = 13,
        VTK_PYRAMID          = 14,
        VTK_PENTAGONAL_PRISM = 15,
        VTK_HEXAGONAL_PRISM  = 16,
        VTK_POLYHEDRON       = 42
    };


    //- Some common XML tags for vtk files
    enum class fileTag : uint8_t
    {
        UNKNOWN,                //!< placeholder
        VTK_FILE,               //!< "VTKFile"
        DATA_ARRAY,             //!< "DataArray"
        BLOCK,                  //!< "Block"
        PIECE,                  //!< "Piece"
        DATA_SET,               //!< "DataSet"
        POINTS,                 //!< "Points"
        CELLS,                  //!< "Cells"
        POLYS,                  //!< "Polys"
        VERTS,                  //!< "Verts"
        LINES,                  //!< "Lines"
        CELL_DATA,              //!< "CellData"
        POINT_DATA,             //!< "PointData"
        FIELD_DATA,             //!< "FieldData"
        POLY_DATA,              //!< "PolyData"
        UNSTRUCTURED_GRID,      //!< "UnstructuredGrid"
        MULTI_BLOCK,            //!< "vtkMultiBlockDataSet"
        VTK_HDF,                //!< "VTKHDF" - used for the extension
    };

    //- File extension (without ".") for some vtk XML file content types
    extern fileFormats_API const Foam::Enum<fileTag> fileExtension;

    //- Version string for some vtk XML file content types
    extern fileFormats_API const Foam::Enum<fileTag> fileContentVersions;

    //- Strings corresponding to the vtk XML tags
    extern fileFormats_API const Foam::Enum<fileTag> fileTagNames;

    //- Some common XML attributes for vtk files
    enum class fileAttr : uint8_t
    {
        OFFSET,                 //!< "offset"
        NUMBER_OF_COMPONENTS,   //!< "NumberOfComponents"
        NUMBER_OF_TUPLES,       //!< "NumberOfTuples"
        NUMBER_OF_POINTS,       //!< "NumberOfPoints"
        NUMBER_OF_CELLS,        //!< "NumberOfCells"
        NUMBER_OF_POLYS,        //!< "NumberOfPolys"
        NUMBER_OF_VERTS,        //!< "NumberOfVerts"
        NUMBER_OF_LINES,        //!< "NumberOfLines"
    };

    //- Strings corresponding to the vtk XML attributes
    extern fileFormats_API const Foam::Enum<fileAttr> fileAttrNames;

    //- Some common names for XML DataArray entries
    enum class dataArrayAttr : uint8_t
    {
        POINTS,                 //!< "Points"
        OFFSETS,                //!< "offsets"
        CONNECTIVITY,           //!< "connectivity"
        TYPES,                  //!< "types"
        FACES,                  //!< "faces"
        FACEOFFSETS,            //!< "faceoffsets"
    };

    //- Strings corresponding to the vtk XML DataArray attributes
    extern fileFormats_API const Foam::Enum<dataArrayAttr> dataArrayAttrNames;


/*---------------------------------------------------------------------------*\
                               Namespace legacy
\*---------------------------------------------------------------------------*/

namespace legacy
{

    //- Legacy file extension ("vtk")
    extern fileFormats_API const word fileExtension;

    //- Legacy content names (POLYDATA, UNSTRUCTURED_GRID)
    extern fileFormats_API const Foam::Enum<vtk::fileTag> contentNames;

    //- Legacy file tags (eg, LINES, CELL_DATA, POINT_DATA, ...)
    extern fileFormats_API const Foam::Enum<vtk::fileTag> fileTagNames;

    //- Legacy attributes (eg, OFFSETS)
    extern fileFormats_API const Foam::Enum<dataArrayAttr> dataArrayAttrNames;

} // End namespace Foam::vtk::legacy


/*---------------------------------------------------------------------------*\
                               Namespace Tools
\*---------------------------------------------------------------------------*/

namespace Tools
{

//- Return a copy of the \p value, with components changed
//- from OpenFOAM order to VTK order (for symmTensor)
template<class Type>
Type copyTuple(const Type& value);

//- Implementation detail - not for general use!
template<class FloatType, class Type>
const FloatType* copyTuple_impl(FloatType output[], const Type& value);


//- Copy/transcribe components of OpenFOAM \p value to the \p output buffer
//- (which must be large enough to contain all components)
//
//  \note When copying, the components of Foam::symmTensor are reordering
//  to match the VTK order:
//   - OpenFOAM : (XX, XY, XZ, YY, YZ, ZZ)
//   - VTK order: (XX, YY, ZZ, XY, YZ, XZ)
//
//  \returns the pointer to \p output buffer, which allows this routine
//  to act as pass-through filter.
template<class Type>
const double* copyTuple
(
    double output[],  //!< [out] the output/work buffer
    const Type& value
);

//- Copy/transcribe components of OpenFOAM \p value to the \p output buffer
//- (which must be large enough to contain all components)
template<class Type>
const float* copyTuple
(
    float output[],  //!< [out] the output/work buffer
    const Type& value
);

//- Inplace component reordering of \p value from OpenFOAM order to VTK order
//  Reordering is currently only applied for Foam::symmTensor,
//  all other types are a no-op.
//
//  \sa copyTuple
template<class Type>
void reorderTuple(Type& value);

} // End namespace Foam::vtk::Tools

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace vtk
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "foamVtkCore.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
