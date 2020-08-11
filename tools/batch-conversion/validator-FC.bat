for %%f in (1-input\*.bin) do @FC /B %%f 5-output\%%~nf.bin > fc_logs\%%~nf_log.txt
pause