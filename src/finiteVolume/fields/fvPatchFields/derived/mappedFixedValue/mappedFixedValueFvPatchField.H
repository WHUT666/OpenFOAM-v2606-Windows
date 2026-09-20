/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2018 OpenCFD Ltd.
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
    Foam::mappedFixedValueFvPatchField

Group
    grpGenericBoundaryConditions grpCoupledBoundaryConditions

Description
    This boundary condition maps the value at a set of cells or patch faces
    back to *this.

    The sample mode is set by the underlying mapping engine, provided by the
    mappedPatchBase class.

Usage
    \table
        Property     | Description                      | Required | Default
        field        | Field name to be mapped          | no | this field name
        setAverage   | Use average value                | no | false
        average      | Average value to use if \c setAverage = yes | partly |
        interpolationScheme | type of interpolation scheme | partly |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            mapped;
        field           T;
        setAverage      no;
        average         0;
        interpolationScheme cell;
        value           uniform 0;
    }
    \endverbatim

    When employing the \c nearestCell sample mode, the user must also specify
    the interpolation scheme using the \c interpolationScheme entry.

    In case of interpolation (where scheme != cell) the limitation is that
    there is only one value per cell.  For example, if you have a cell with two
    boundary faces and both faces sample into the cell, both faces will get the
    same value.

Note
    It is not possible to sample internal faces since volume fields are not
    defined on faces.

See also
    Foam::mappedPatchBase
    Foam::interpolation
    Foam::fixedValueFvPatchField

SourceFiles
    mappedFixedValueFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_mappedFixedValueFvPatchField_defines_typeName in .C files that
//  define the template statics of mappedFixedValueFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_mappedFixedValueFvPatchField_defines_typeName)
#  define Foam_mappedFixedValueFvPatchField_typeName_API
#else
#  define Foam_mappedFixedValueFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_mappedFixedValueFvPatchField_H
#define Foam_mappedFixedValueFvPatchField_H

#include "fixedValueFvPatchFields.H"
//#include "interpolation.H"
#include "mappedPatchFieldBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class mappedFixedValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mappedFixedValueFvPatchField
:
    public fixedValueFvPatchField<Type>,
    public mappedPatchFieldBase<Type>
{
    typedef mappedFixedValueFvPatchField<Type> this_bctype;
    typedef fixedValueFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_mappedFixedValueFvPatchField_typeName_API, "mapped");


    // Constructors

        //- Construct from patch and internal field
        mappedFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        mappedFixedValueFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        mappedFixedValueFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        mappedFixedValueFvPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "mappedFixedValueFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
