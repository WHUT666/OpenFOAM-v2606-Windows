/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2024-2025 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "surfaceCourantNumber.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "faMesh.H"
#include "fvMesh.H"
#include "areaFields.H"
#include "edgeFields.H"
#include "facEdgeIntegrate.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(surfaceCourantNumber, 0);
    addToRunTimeSelectionTable
    (
        functionObject,
        surfaceCourantNumber,
        dictionary
    );
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::functionObjects::surfaceCourantNumber::writeFileHeader(Ostream& os)
{
    writeHeader(os, "Surface Courant Number");

    writeCommented(os, "Time");
    writeTabbed(os, "min");
    writeTabbed(os, "max");
    writeTabbed(os, "mean");
    os  << endl;

    writtenHeader_ = true;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::surfaceCourantNumber::surfaceCourantNumber
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObjects::fvMeshFunctionObject(name, runTime, dict),
    writeFile(mesh_, name, typeName, dict),
    resultName_("surfaceCo"),
    phisName_("phis"),
    rhoName_("rho"),
    faMeshPtr_(nullptr)
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::surfaceCourantNumber::read(const dictionary& dict)
{
    if (!fvMeshFunctionObject::read(dict) || !writeFile::read(dict))
    {
        return false;
    }

    dict.readIfPresent("result", resultName_);
    dict.readIfPresent("phis", phisName_);
    dict.readIfPresent("rho", rhoName_);

    // Registry containing all finite-area meshes on the polyMesh
    const auto* faRegistry = faMesh::registry(mesh_);

    if (!faRegistry)
    {
        FatalIOErrorInFunction(dict)
            << "No finite-area object registry is available."
            << abort(FatalIOError);
    }

    word areaName;
    if (!dict.readIfPresent("area", areaName))
    {
        wordList available = faRegistry->sortedNames<faMesh>();
        if (!available.empty())
        {
            areaName = available.front();
        }
    }

    faMeshPtr_ = faRegistry->cfindObject<faMesh>(areaName);

    if (!faMeshPtr_)
    {
        FatalIOErrorInFunction(dict)
            << "No finite-area mesh available."
            << abort(FatalIOError);
    }

    return true;
}


bool Foam::functionObjects::surfaceCourantNumber::execute()
{
    const auto* phiPtr = faMeshPtr_->cfindObject<edgeScalarField>(phisName_);

    if (!phiPtr)
    {
        WarningInFunction
            << "No edge flux field is available. "
            << "Name of provided edge flux field (phi): " << phisName_
            << endl;

        return false;
    }

    const auto& phi = *phiPtr;

    tmp<areaScalarField::Internal> tCo =
    (
        (0.5*faMeshPtr_->time().deltaT())
      * fac::edgeSum(Foam::mag(phi))().internalField()
      / faMeshPtr_->S()
    );

    if (tCo().dimensions() == dimDensity)
    {
        tCo.ref() /= faMeshPtr_->lookupObject<areaScalarField>(rhoName_);
    }


    auto* resultPtr = faMeshPtr_->getObjectPtr<areaScalarField>(resultName_);

    if (!resultPtr)
    {
        resultPtr = new areaScalarField
        (
            IOobject
            (
                resultName_,
                faMeshPtr_->time().timeName(),
               *faMeshPtr_,
                IOobjectOption()
            ),
            *faMeshPtr_,
            dimensionedScalar(dimless, Zero),
            faPatchFieldBase::zeroGradientType()
        );
        regIOobject::store(resultPtr);
    }
    auto& result = *resultPtr;

    result.internalFieldRef() = tCo;
    result.correctBoundaryConditions();


    const scalarMinMax limits = gMinMax(result.primitiveField());
    const scalar mean = gAverage(result.primitiveField());

    Log << "Surface Courant number: "
        << "mean: " << mean
        << " max: " << limits.max()
        << endl;

    if (writeToFile())
    {
        if (!writtenHeader_) writeFileHeader(file());

        writeCurrentTime(file());
        file()
            << token::TAB << limits.min()
            << token::TAB << limits.max()
            << token::TAB << mean
            << endl;
    }

    return true;
}


bool Foam::functionObjects::surfaceCourantNumber::write()
{
    const auto* result = faMeshPtr_->cfindObject<areaScalarField>(resultName_);

    if (!result)
    {
        return false;
    }

    Log << type() << " " << name() << " write: " << result->name() << endl;

    result->write();

    return true;
}


// ************************************************************************* //
