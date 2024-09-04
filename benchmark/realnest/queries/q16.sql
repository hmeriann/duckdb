SELECT 
  list_aggregate(tau_pt, 'kurtosis'),
  list_aggregate(tau_eta, 'kurtosis'),
  list_aggregate(jet_pt, 'kurtosis'),
  list_aggregate(jet_eta, 'kurtosis'),
  list_aggregate(muon_pt, 'kurtosis'),
  list_aggregate(muon_eta, 'kurtosis'),
  list_aggregate(el_pt, 'kurtosis'),
  list_aggregate(el_eta, 'kurtosis'),
  list_aggregate(ph_pt, 'kurtosis'),
  list_aggregate(ph_eta, 'kurtosis')  
FROM singleMu;