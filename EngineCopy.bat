robocopy ".\Project\Engine" ".\External\Include\Engine" *.h *.inl /S /NJH /NJS /NFL /NDL
robocopy ".\Project\Engine" ".\OutputFile\content\shader" *.fx /S /NJH /NJS /NFL /NDL
exit /B 0
