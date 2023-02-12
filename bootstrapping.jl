using Printf
using Random
using StatsBase
using LinearAlgebra
using BenchmarkTools

f(x, p) = p[1] .+ p[2]*x .+ randn(size(x))*0.1.*x

x = collect(0:0.01:10-0.01)
p_true = (0, 1)
y = f(x, p_true);

model(x, p) = p[1] .+ p[2]*x

function fit(
    x::Vector{Float64},
    y::Vector{Float64}
)::Vector{Float64}
    X = hcat(ones(size(x)), x)
    return X \ y
end

fit(x, y)

println("Threads ", Threads.nthreads())

function bootstrap(
    x::Vector{Float64},
    y::Vector{Float64};
    num_iters::Integer = 1000
)::Matrix{Float64}
    p_boot = zeros(num_iters, 2)
    size_x = size(x)
    Threads.@threads for i in 1:num_iters
        idx_boot = sample(range(1, size_x[1]), size_x)
        x_boot = x[idx_boot]
        y_boot = y[idx_boot]

        p = fit(x_boot, y_boot)

        p_boot[i, :] = p
    end
    return p_boot
end

# warm-up
p_boot = bootstrap(x, y; num_iters=10);

b = @benchmark begin
    p_boot = bootstrap(x, y; num_iters=100_000)
end

println(b)

@printf "mean: %.3f %.3f\n" mean(p_boot, dims=1)[1] mean(p_boot, dims=1)[2]
@printf "std:  %.3f %.3f\n" std(p_boot, dims=1)[1] std(p_boot, dims=1)[2]

@printf "percentile p_1: %.3f %.3f\n" percentile(p_boot[:,1], 2.5) percentile(p_boot[:,1], 97.5)

@printf "percentile p_2: %.3f %.3f\n" percentile(p_boot[:,2], 2.5) percentile(p_boot[:,2], 97.5)
