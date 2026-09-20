/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2021 OpenCFD Ltd.
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
    Foam::exprValuePointPatchField

Description
    A fixed value point boundary condition with expressions.

Usage
    \table
        Property     | Description                          | Required | Default
        valueExpr    | expression for uniformValue          | yes |
        value        | initial field value                  | optional |
    \endtable

    The \c value entry (optional) is used for the initial values.
    Otherwise uses zero-gradient values.

SourceFiles
    exprValuePointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_exprValuePointPatchField_defines_typeName in .C files that
//  define the template statics of exprValuePointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_exprValuePointPatchField_defines_typeName)
#  define Foam_exprValuePointPatchField_typeName_API
#else
#  define Foam_exprValuePointPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_exprValuePointPatchField_H
#define Foam_exprValuePointPatchField_H

#include "valuePointPatchField.H"
#include "patchExprFieldBase.H"
#include "patchExprDriver.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class exprValuePointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class exprValuePointPatchField
:
    public valuePointPatchField<Type>,
    public expressions::patchExprFieldBase
{
    typedef exprValuePointPatchField<Type> this_bctype;
    typedef valuePointPatchField<Type> parent_bctype;


protected:

    // Protected Data

        //- Dictionary contents for the boundary condition
        dictionary dict_;

        //- The expression driver
        expressions::patchExpr::parseDriver driver_;


public:

    //- Runtime type information
    TypeNameApi(Foam_exprValuePointPatchField_typeName_API, "exprValue");


    // Constructors

        //- Construct from patch and internal field
        exprValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        exprValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        exprValuePointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        exprValuePointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        exprValuePointPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return pointPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Update the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream& os) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "exprValuePointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
