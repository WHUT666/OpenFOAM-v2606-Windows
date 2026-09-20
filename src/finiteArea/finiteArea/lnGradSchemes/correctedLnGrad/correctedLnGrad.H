/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
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
    Foam::fa::correctedLnGrad

Description
    Simple central-difference lnGrad scheme with non-orthogonal correction.

SourceFiles
    correctedLnGrad.C

\*---------------------------------------------------------------------------*/

//- Define Foam_correctedLnGrad_defines_typeName in .C files that
//  define the template statics of correctedLnGrad (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_correctedLnGrad_defines_typeName)
#  define Foam_correctedLnGrad_typeName_API
#else
#  define Foam_correctedLnGrad_typeName_API finiteArea_API
#endif

#ifndef correctedLnGrad_H
#define correctedLnGrad_H

#include "lnGradScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fa
{

/*---------------------------------------------------------------------------*\
                       Class correctedLnGrad Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class correctedLnGrad
:
    public lnGradScheme<Type>
{
public:

    //- Runtime type information
    TypeNameApi(Foam_correctedLnGrad_typeName_API, "corrected");


    // Generated Methods

        //- No copy construct
        correctedLnGrad(const correctedLnGrad&) = delete;

        //- No copy assignment
        void operator=(const correctedLnGrad&) = delete;


    // Constructors

        //- Construct from mesh
        correctedLnGrad(const faMesh& mesh)
        :
            lnGradScheme<Type>(mesh)
        {}


        //- Construct from mesh and data stream
        correctedLnGrad(const faMesh& mesh, Istream&)
        :
            lnGradScheme<Type>(mesh)
        {}


    //- Destructor
    virtual ~correctedLnGrad() = default;


    // Member Functions

        //- Return the interpolation weighting factors for the given field
        virtual tmp<edgeScalarField> deltaCoeffs
        (
            const GeometricField<Type, faPatchField, areaMesh>&
        ) const
        {
            return this->mesh().deltaCoeffs();
        }

        //- Return true if this scheme uses an explicit correction
        virtual bool corrected() const
        {
            return !this->mesh().orthogonal();
        }

        //- Return the explicit correction to the correctedLnGrad
        //  for the given field
        virtual tmp<GeometricField<Type, faePatchField, edgeMesh>>
        correction(const GeometricField<Type, faPatchField, areaMesh>&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fa

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "correctedLnGrad.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
