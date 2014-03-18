program E710com;

uses
  Forms,
  UM850 in '..\E-710 Com- Objekt fortgeschickt\UM850.pas' {Form1},
  M850_COMLib_TLB in '..\..\..\..\Programme\Borland\Delphi7\Imports\M850_COMLib_TLB.pas',
  E710_GCS_COMLib_TLB in '..\..\Imports\E710_GCS_COMLib_TLB.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
