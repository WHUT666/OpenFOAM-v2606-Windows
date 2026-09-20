/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2024 OpenCFD Ltd.
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
    Foam::PatchFunction1Types::LookupField

Description
    PatchFunction1 to lookup volField/surfaceField or pointField from an
    objectregistry and return its value on the patch.

    The dictionary specification would typically resemble this:
    \verbatim
    entry
    {
        type        lookup;
        name        myField;
    }
    \endverbatim

    where the entries mean:
    \table
        Property  | Description                 | Type | Reqd | Default
        type      | Function type: lookup       | word | yes |
        name      | name of volField/pointField | word | yes |
    \endtable

SourceFiles
    LookupField.C

\*---------------------------------------------------------------------------*/

//- Define Foam_LookupField_defines_typeName in .C files that
//  define the template statics of LookupField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_LookupField_defines_typeName)
#  define Foam_LookupField_typeName_API
#else
#  define Foam_LookupField_typeName_API finiteVolume_API
#endif

#ifndef PatchFunction1Types_LookupField_H
#define PatchFunction1Types_LookupField_H

#include "PatchFunction1.H"
#include "lookupBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace PatchFunction1Types
{

/*---------------------------------------------------------------------------*\
                            Class LookupField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class LookupField
:
    public PatchFunction1<Type>,
    protected lookupBase
{


public:

    // Runtime type information
    TypeName("lookup");


    // Generated Methods

        //- No copy assignment
        void operator=(const LookupField<Type>&) = delete;


    // Constructors

        //- Construct from entry name, dictionary and optional registry
        LookupField
        (
            const polyPatch& pp,
            const word& redirectType,
            const word& entryName,
            const dictionary& dict,
            const bool faceValues = true
        );

        //- Copy construct, setting patch
        explicit LookupField
        (
            const LookupField<Type>& rhs,
            const polyPatch& pp
        );

        //- Copy construct
        explicit LookupField(const LookupField<Type>& rhs);

        //- Construct and return a clone
        virtual tmp<PatchFunction1<Type>> clone() const
        {
            return tmp<PatchFunction1<Type>>(new LookupField<Type>(*this));
        }

        //- Return a clone, setting the patch
        virtual tmp<PatchFunction1<Type>> clone(const polyPatch& pp) const
        {
            return PatchFunction1<Type>::Clone(*this, pp);
        }


    //- Destructor
    virtual ~LookupField() = default;


    // Member Functions

        //- Is value uniform (i.e. independent of coordinate)
        virtual inline bool uniform() const { return false; }


        // Evaluation

            //- Return Lookup value
            virtual tmp<Field<Type>> value(const scalar x) const;

            //- Integrate between two values
            virtual tmp<Field<Type>> integrate
            (
                const scalar x1,
                const scalar x2
            ) const;


        // I-O

            //- Write in dictionary format
            virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace PatchFunction1Types
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "LookupField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

finiteVolume_TEMPLATE_IMPORT(Foam::PatchFunction1Types::LookupField<Foam::scalar>);
finiteVolume_TEMPLATE_IMPORT(Foam::PatchFunction1Types::LookupField<Foam::vector>);
finiteVolume_TEMPLATE_IMPORT(Foam::PatchFunction1Types::LookupField<Foam::sphericalTensor>);
finiteVolume_TEMPLATE_IMPORT(Foam::PatchFunction1Types::LookupField<Foam::symmTensor>);
finiteVolume_TEMPLATE_IMPORT(Foam::PatchFunction1Types::LookupField<Foam::tensor>);

#endif

// ************************************************************************* //
