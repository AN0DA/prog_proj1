Option Compare Database

Private Sub Dodaj_ucznia_dodaj_rekord_Click()
    Dim Imie As String
    Dim Nazwisko As String
    Dim Kod As Long
    Dim Klasa As String

    Imie = Me.Tekst40.Value
    Nazwisko = Me.Tekst42.Value
    Klasa = Me.Tekst44.Value
    Kod = Me.Tekst46.Value

    CurrentDb.Execute "INSERT INTO uczniowie (imie, nazwisko, klasa, kod) " &
     "VALUES ('" & Imie & "', '" & Nazwisko & "', '" & Klasa & "', " & Kod & ");", dbFailOnError + dbSeeChanges

 Funkcje_globalne.Wyswietl_info("Dodano ucznia [KOD = " & Kod & "]")

    DoCmd.Close
End Sub

Private Sub Form_Load()
    Dim Temp As Variant

    Temp = DCount("[ID_ucznia]", "uczniowie") + 1

    Me.Tekst38 = Temp

    Me.Tekst46 = KOD_global

    Me.Refresh
End Sub

