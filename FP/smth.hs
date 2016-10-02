integration :: (Double -> Double) -> Double -> Double -> Double
integration f a b = helper 5000 f a b
				where helper n f a b | n == 1 = (b-a) * (f a + f b) / 2
				                     | otherwise = helper (div n 2) f a ((b+a)/2) + helper (div (n+1) 2) f ((b+a)/2) b