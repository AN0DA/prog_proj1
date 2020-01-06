Option Compare Database

Function Znajdz_ucznia(Kod) As String

    Dim Current_id As String
    Dim Current_imie As String
    Dim Current_nazwisko As String
    Dim Current_klasa As String
    Dim Current_ID_ucznia As String

    Current_id = DLookup("[ID_ucznia]", "uczniowie", "kod = " & Kod)
    current_name = DLookup("[imie]", "uczniowie", "kod = " & Kod)
    Current_nazwisko = DLookup("[nazwisko]", "uczniowie", "kod = " & Kod)
    Current_klasa = DLookup("[klasa]", "uczniowie", "kod = " & Kod)

    Znajdz_ucznia = "ID = " & Current_id & vbNewLine & current_name & " " & Current_nazwisko & vbNewLine & "Klasa " & Current_klasa & vbNewLine & "KOD = " & Kod

End Function


Function Znajdz_ucznia_id(ID As Long) As String

    Dim Current_imie As String
    Dim Current_nazwisko As String
    Dim Current_klasa As String
    Dim Current_kod As String

    current_name = DLookup("[imie]", "uczniowie", "ID_ucznia = " & ID)
    Current_nazwisko = DLookup("[nazwisko]", "uczniowie", "ID_ucznia = " & ID)
    Current_klasa = DLookup("[klasa]", "uczniowie", "ID_ucznia = " & ID)
    Current_kod = DLookup("[ID_ucznia]", "uczniowie", "ID_ucznia = " & ID)

    Znajdz_ucznia_id = "ID = " & ID & vbNewLine & current_name & " " & Current_nazwisko & vbNewLine & "Klasa " & Current_klasa & vbNewLine & "KOD = " & Current_kod

End Function


Function Update_liczba() As Integer
    Dim Temp As Variant

    Temp = DCount("[ID_ucznia]", "obecnie_swietlica")
    Forms!menu!Etykieta30.Caption = Temp & "/" & Pojemnosc

    Temp = DCount("[ID_ucznia]", "obecnie_stolowka")
    Forms!menu!Etykieta33.Caption = Temp

End Function

Sub Wyswietl_info(msg As String)

    Dim varReturn As Variant
    Dim TWait As Variant
    Dim TNow As Variant

    varReturn = SysCmd(acSysCmdSetStatus, msg)

    TWait = Time
    TWait = DateAdd("s", 3, TWait)
    Do Until TNow >= TWait
        TNow = Time
    Loop


    varReturn = SysCmd(acSysCmdSetStatus, "Amnesty 2019 - ZSK Edition")

End Sub



