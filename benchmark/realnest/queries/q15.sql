SELECT 
  list_aggregate(tau_pt, 'kurtosis_pop'),
  list_aggregate(tau_eta, 'kurtosis_pop'),
  list_aggregate(jet_pt, 'kurtosis_pop'),
  list_aggregate(jet_eta, 'kurtosis_pop'),
  list_aggregate(muon_pt, 'kurtosis_pop'),
  list_aggregate(muon_eta, 'kurtosis_pop'),
  list_aggregate(el_pt, 'kurtosis_pop'),
  list_aggregate(el_eta, 'kurtosis_pop'),
  list_aggregate(ph_pt, 'kurtosis_pop'),
  list_aggregate(ph_eta, 'kurtosis_pop')  
FROM singleMu;