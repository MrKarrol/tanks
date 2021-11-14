#pragma once

#include "TGun.h"

#include "TTraceGun.generated.h"


UCLASS()
class ATTraceGun : public ATGun
{
	GENERATED_BODY()
public:

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		float TraceDistance = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		bool bDrawDebugTrace = false;

protected:
	void DoFire() override;

};