// Copyright YYJ


#include "GTAnimNotify.h"

FString UGTAnimNotify::GetNotifyName_Implementation() const
{
	FString NotifyName;

#if WITH_EDITORONLY_DATA
	if (UObject* ClassGeneratedBy = GetClass()->ClassGeneratedBy)
	{
		// GeneratedBy will be valid for blueprint types and gives a clean name without a suffix
		NotifyName = ClassGeneratedBy->GetName();
	}
	else
#endif
	{
		// Native notify classes are clean without a suffix otherwise
		NotifyName = GetClass()->GetName();
	}

	NotifyName.ReplaceInline(TEXT("GTAnimNotify_"), TEXT("GT"), ESearchCase::CaseSensitive);
	
	return NotifyName;
}
