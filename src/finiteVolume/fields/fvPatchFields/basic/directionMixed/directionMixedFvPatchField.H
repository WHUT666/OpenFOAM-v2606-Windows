/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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
    Foam::directionMixedFvPatchField

Group
    grpGenericBoundaryConditions

Description
    Base class for direction-mixed boundary conditions.

SourceFiles
    directionMixedFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_directionMixedFvPatchField_defines_typeName in .C files that
//  define the template statics of directionMixedFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_directionMixedFvPatchField_defines_typeName)
#  define Foam_directionMixedFvPatchField_typeName_API
#else
#  define Foam_directionMixedFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_directionMixedFvPatchField_H
#define Foam_directionMixedFvPatchField_H

#include "transformFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class directionMixedFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class directionMixedFvPatchField
:
    public transformFvPatchField<Type>
{
    typedef directionMixedFvPatchField<Type> this_bctype;
    typedef transformFvPatchField<Type> parent_bctype;

    // Private Data

        //- Value field
        Field<Type> refValue_;

        //- Normal gradient field
        Field<Type> refGrad_;

        //- Fraction (0-1) of value used for boundary condition
        symmTensorField valueFraction_;


public:

    //- Runtime type information
    TypeNameApi(Foam_directionMixedFvPatchField_typeName_API, "directionMixed");


    // Constructors

        //- Construct from patch and internal field
        directionMixedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        directionMixedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        directionMixedFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        directionMixedFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        directionMixedFvPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- True: this patch field fixes a value.
        virtual bool fixesValue() const { return true; }

        //- False: this patch field is not altered by assignment.
        virtual bool assignable() const { return false; }


    // Mapping Functions

        //- Map (and resize as needed) from self given a mapping object
        virtual void autoMap(const fvPatchFieldMapper&);

        //- Reverse map the given fvPatchField onto this fvPatchField
        virtual void rmap(const fvPatchField<Type>&, const labelList&);


        // Return defining fields

            virtual Field<Type>& refValue()
            {
                return refValue_;
            }

            virtual const Field<Type>& refValue() const
            {
                return refValue_;
            }

            virtual Field<Type>& refGrad()
            {
                return refGrad_;
            }

            virtual const Field<Type>& refGrad() const
            {
                return refGrad_;
            }

            virtual symmTensorField& valueFraction()
            {
                return valueFraction_;
            }

            virtual const symmTensorField& valueFraction() const
            {
                return valueFraction_;
            }


        // Evaluation functions

            //- Return gradient at boundary
            virtual tmp<Field<Type>> snGrad() const;

            //- Retrieve patch-normal gradient [contiguous storage]
            virtual void snGrad(UList<Type>& result) const;

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType =
                    Pstream::commsTypes::buffered
            );

            //- Return face-gradient transform diagonal
            virtual tmp<Field<Type>> snGradTransformDiag() const;


        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        virtual void operator=(const UList<Type>&) {}

        virtual void operator=(const fvPatchField<Type>&) {}
        virtual void operator+=(const fvPatchField<Type>&) {}
        virtual void operator-=(const fvPatchField<Type>&) {}
        virtual void operator*=(const fvPatchField<scalar>&) {}
        virtual void operator/=(const fvPatchField<scalar>&) {}

        virtual void operator+=(const Field<Type>&) {}
        virtual void operator-=(const Field<Type>&) {}
        virtual void operator*=(const Field<scalar>&) {}
        virtual void operator/=(const Field<scalar>&) {}

        virtual void operator=(const Type&) {}
        virtual void operator+=(const Type&) {}
        virtual void operator-=(const Type&) {}
        virtual void operator*=(const scalar) {}
        virtual void operator/=(const scalar) {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "directionMixedFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
