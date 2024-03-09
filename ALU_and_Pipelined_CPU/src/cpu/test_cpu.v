module test_cpu();
    reg CLK;
    PipelinedCPU dut(.CLK(CLK));
    always begin
        CLK=0; #10;
        CLK=1; #10;
    end
endmodule
