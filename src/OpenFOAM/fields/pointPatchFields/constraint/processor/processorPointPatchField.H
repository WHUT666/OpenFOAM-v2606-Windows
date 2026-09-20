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
    Foam::processorPointPatchField

Description
    Foam::processorPointPatchField

SourceFiles
    processorPointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_processorPointPatchField_defines_typeName in .C files that
//  define the template statics of processorPointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_processorPointPatchField_defines_typeName)
#  define Foam_processorPointPatchField_typeName_API
#else
#  define Foam_processorPointPatchField_typeName_API OpenFOAM_API
#endif

#ifndef Foam_processorPointPatchField_H
#define Foam_processorPointPatchField_H

#include "coupledPointPatchField.H"
#include "processorPointPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                 Class processorPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class processorPointPatchField
:
    public coupledPointPatchField<Type>
{
    typedef processorPointPatchField<Type> this_bctype;
    typedef coupledPointPatchField<Type> parent_bctype;

    // Private data

        //- Local reference to processor patch
        const processorPointPatch& procPatch_;

public:

    //- Runtime type information
    TypeNameApi(Foam_processorPointPatchField_typeName_API, processorPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        processorPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        processorPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        processorPointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        processorPointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        processorPointPatchField(const this_bctype& pfld)
        #ifdef Foam_pointPatchField_copyConstruct
        : this_bctype(pfld, pfld.internalField()) {}
        #else
        = delete;
        #endif

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return pointPatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~processorPointPatchField() = default;


    // Member functions

        // Access

            //- Return true if running parallel
            virtual bool coupled() const
            {
                return UPstream::parRun();
            }

            //- Does the patch field perform the transformation
            virtual bool doTransform() const
            {
                return
                (
                    is_rotational_vectorspace_v<Type>
                 && !procPatch_.procPolyPatch().parallel()
                );
            }


        // Constraint handling

            //- Return the constraint type this pointPatchField implements
            virtual const word& constraintType() const
            {
                return type();
            }


        // Evaluation functions

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType =
                    Pstream::commsTypes::buffered
            )
            {}

            //- Assume processor patch always collocated
            virtual void swapAddSeparated
            (
                const Pstream::commsTypes commsType,
                Field<Type>&
            ) const
            {}


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "processorPointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
