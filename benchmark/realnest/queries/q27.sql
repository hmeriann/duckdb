SELECT 
  list_aggregate(tau_pt, 'skewness'),
  list_aggregate(tau_eta, 'skewness'),
  list_aggregate(jet_pt, 'skewness'),
  list_aggregate(jet_eta, 'skewness'),
  list_aggregate(muon_pt, 'skewness'),
  list_aggregate(muon_eta, 'skewness'),
  list_aggregate(el_pt, 'skewness'),
  list_aggregate(el_eta, 'skewness'),
  list_aggregate(ph_pt, 'skewness'),
  list_aggregate(ph_eta, 'skewness')  
FROM singleMu;