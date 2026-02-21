// Fill out your copyright notice in the Description page of Project Settings.


#include "Interchange/SGMarkdownExporter.h"

#include "Core/SGSetup.h"
USGMarkdownExporter::USGMarkdownExporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = USGSetup::StaticClass();
	bText = true;
	PreferredFormatIndex = 0;
	FormatExtension.Add(TEXT("MD"));
	FormatDescription.Add(TEXT("Style Guide Markdown"));
}

bool USGMarkdownExporter::ExportText(const FExportObjectInnerContext* Context, UObject* Object, const TCHAR* Type,
	FOutputDevice& Ar, FFeedbackContext* Warn, uint32 PortFlags)
{
	auto Setup = Cast<USGSetup>(Object);
	if (!Setup)
	{
		return false;
	}
	Ar.Logf( TEXT("%s#Style Guide Setup\r\n"), FCString::Spc(TextIndent) );
	
	/*if (!(PortFlags & PPF_SeparateDeclare))
	{
		UExporter::ExportToOutputDevice(Context, Model->Polys, NULL, Ar, Type, TextIndent + 3, PortFlags);
	}*/
	return true;
}